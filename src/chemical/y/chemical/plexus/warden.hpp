
//! \file

#ifndef Y_Chemical_Warden_Included
#define Y_Chemical_Warden_Included 1

#include "y/chemical/plexus/warden/trims.hpp"
#include "y/chemical/plexus/warden/fixed.hpp"
#include "y/chemical/plexus/warden/trade.hpp"
#include "y/chemical/plexus/cluster.hpp"

#include "y/system/exception.hpp"
#include "y/sort/heap.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Warden: fix laws and equalize
        //
        //
        //______________________________________________________________________
        class Warden : public Quantized
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Conservation::Laws::Group Group; //!< alias
            typedef Conservation::LNode       LNode; //!< alias
            typedef Conservation::LRepo       LRepo; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Warden(const Cluster &cluster); //!< setup from cluster
            virtual ~Warden() noexcept;              //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void prolog() noexcept; //! free all injected

            //! sanitize   equilibria
            void sanitize(XWritable &C, const Level L, XMLog &xml)
            {

                Y_XML_SECTION(xml, "sanitize" );

                //--------------------------------------------------------------
                //
                // inject corrections and detect zero laws
                //
                //--------------------------------------------------------------
                lawz.free();
                for(const Group *g=head;g;g=g->next)
                {
                    const Group                    &G   = *g;
                    const Conservation::Law * const law = wasInjected(G,C,L,xml);
                    if(0!=law)
                        lawz << *law;
                }

                //--------------------------------------------------------------
                //
                // check CONSERVED with LIMITED
                //
                //--------------------------------------------------------------
                if(0!=head)
                {
                    if(lawz.size && xml.verbose)
                    {
                        Y_XML_SECTION(xml, "z-laws");
                        for(const LNode *ln=lawz.head;ln;ln=ln->next)
                        {
                            xml() << **ln << std::endl;
                        }
                    }
                    const SNode * const node = mine.conserved.list.head;

                    if(xml.verbose)
                    {
                        for(const SNode *sn = node;sn;sn=sn->next)
                        {
                            const Species &sp = **sn;
                            xml() << "d[" << sp << "]=" << cinj[sp.indx[SubLevel]] << std::endl;
                        }
                    }

                    // check is any conserved is negative
                    for(const SNode *sn=node;sn;sn=sn->next)
                    {
                        if( C[ (**sn).indx[L]].mantissa < 0 ) goto EQUALIZE;
                    }
                    return;

                EQUALIZE:
                    equalize(C, L, xml);
                }

                //--------------------------------------------------------------
                //
                // and finally adjust UNBOUNDED with ROAMING
                //
                //--------------------------------------------------------------

                // TODO


            }



            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Cluster      &mine;    //!< my cluster
            const Group * const head;    //!< first group
            const size_t        rows;    //!< laws max group size
            const size_t        cols;    //!< max species in sub-level
            XAdd                xadd;    //!< for internal computations
            XMatrix             conc;    //!< workspace for fixed concentrations
            Fixed::Series       jail;    //!< fixed
            XSwell              cinj;    //!< injected accumulator
            Fund                fund;    //!< shared memories
            LRepo               lawz;    //!< laws with zero values
            Trims               trms;    //!< trims
            SingleFrontier      best;    //!< best effort to equalize
            SRepo               wobbly;  //!< negative species list
            XMatrix             ctrade;  //!< traded concentrations
            XMatrix             dtrade;  //!< traded changes
            Trade::Series       trades;  //!< trades
            XSwell              xaccum;  //!< accumulators for multiple trades

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Warden);

            //! equalize CONSERVED with LIMITED equilibria
            /**
             enter with sanitized and possibly lawz
             */
            void equalize(XWritable &C, const Level L, XMLog &xml)
            {
                if(0==head) return;

                Y_XML_SECTION(xml, "equalize" );


                size_t cycle = 0;
            CYCLE:
                ++cycle;
                Y_XMLOG(xml, "-------- #cycle = " << cycle << " --------");

                //--------------------------------------------------------------
                //
                // process limited equilibria
                //
                //--------------------------------------------------------------
                const size_t unbalanced = getUnbalanced(C, L, xml);
                Y_XMLOG(xml, "(#) unbalanced = " << unbalanced);
                if(unbalanced<=0)
                {
                    //----------------------------------------------------------
                    //
                    // numerical succes, check lawz
                    //
                    //----------------------------------------------------------
                    assert(0==trades.size());
                    assert(0==wobbly.size);
                    if(lawz.size>0)
                        enforceZeroLaws(C,L,xml); // will set positive concentrations to zero
                    else
                    {
                        Y_XMLOG(xml, "(#) no z-law to enforce");
                    }
                    return;
                }

                const size_t tradeCount = trades.size();
                Y_XMLOG(xml, "(#) tradeCount = " << tradeCount << " / " << wobbly);
                for(const SNode *sn=wobbly.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    Y_XMLOG(xml, "(!) " << std::setw(15) << real_t( C[sp.indx[L]]) << " = [" << sp << "]");
                }

                if(tradeCount<=0)
                {
                    std::cerr << "No Trade!!" << std::endl;
                    if(!lawz.size)
                    {
                        // bad!
                        throw Specific::Exception("here", "no lawz");
                    }
                    enforceZeroLaws(C,L,xml);
                    std::cerr << "Emergency Exit!!" << std::endl << std::endl;
                    exit(9);
                    return;
                }


                optimizeTrade(C, L, xml);
                goto CYCLE;
            }


            //! enforce law and store delta in injected
            void enforceZeroLaws(XWritable  &C,
                                 const Level L,
                                 XMLog      &xml)
            {
                assert(lawz.size>0);
                Y_XML_SECTION_OPT(xml, "enforce", "size='" << lawz.size << "'");
                for(const LNode *ln=lawz.head;ln;ln=ln->next)
                {
                    const Conservation::Law &law = **ln;
                    Y_XML_SECTION(xml,law.name);
                    for(const Actor *a = law->head;a;a=a->next)
                    {
                        const Species &      sp = a->sp;
                        const size_t * const id = sp.indx;
                        const size_t         J  = id[L];
                        const xreal_t        cc = C[J];
                        Y_XMLOG(xml, std::setw(15) << real_t(cc) << " = [" << sp << "]");
                        switch( Sign::Of(cc.mantissa) )
                        {
                            case __Zero__:
                                break;
                            case Negative:
                            case Positive:
                                cinj[ id[SubLevel] ] << -cc;
                                C[J].ldz();
                                break;
                        }
                    }
                }
            }

            void   optimizeTrade(XWritable &C, const Level L, XMLog &xml)
            {

                Y_XML_SECTION(xml,"optimizeTrade");
                HeapSort::Call(trades,Trade::Compare);
                const Trade &top = trades.head();

                //--------------------------------------------------------------
                //
                // look for successive fully detached trades
                //
                //--------------------------------------------------------------
                {
                    Y_XML_SECTION_OPT(xml, "content", "size='" << trades.size() << "'");

                    Y_XMLOG(xml, "(*) " << top);

                    // keep first trade, look for detached
                    for(size_t i=2;i<=trades.size();)
                    {
                        const Trade          &tri      = trades[i];
                        const Equilibrium    &lhs      = tri.eq;
                        const Readable<bool> &detached = mine.detached[lhs.indx[SubLevel]];
                        bool                  keep     = true;
                        for(size_t j=1;j<i;++j)
                        {
                            const Equilibrium &rhs = trades[j].eq;
                            if(!detached[rhs.indx[SubLevel]])
                            {
                                keep = false;
                                break;
                            }
                        }
                        if(keep)
                        {
                            Y_XMLOG(xml,"(+) " << tri);
                            ++i;
                        }
                        else
                        {
                            Y_XMLOG(xml,"(-) " << tri);
                            trades.remove(i);
                        }
                    }
                }


                //--------------------------------------------------------------
                //
                // apply found trades
                //
                //--------------------------------------------------------------
                {
                    Y_XML_SECTION_OPT(xml, "applied", "size='" << trades.size() << "'");
                    const size_t nt = trades.size(); assert(nt>=1);
                    if(xml.verbose)
                    {
                        for(size_t i=1;i<=nt;++i)
                        {
                            xml() << "($) " << trades[i] << std::endl;
                        }
                    }

                    if(nt<=1)
                    {
                        //------------------------------------------------------
                        // default, transfer
                        //------------------------------------------------------
                        mine.transfer(C, L, top.cc, SubLevel);
                    }
                    else
                    {
                        const AddressBook &cdb = mine.conserved.book;

                        //------------------------------------------------------
                        // initialize accumulator with unbounded species
                        //------------------------------------------------------
                        xaccum.forEach( &XAdd::free );
                        for(const SNode *sn=mine.unbounded.list.head;sn;sn=sn->next)
                        {
                            const Species &      sp = **sn;
                            const size_t * const id = sp.indx;
                            xaccum[ id[SubLevel] ] << C[ id[L] ];
                        }

                        //------------------------------------------------------
                        // dispatch species
                        //------------------------------------------------------
                        for(size_t i=1;i<=nt;++i)
                        {
                            const Trade       &tr = trades[i];
                            const Equilibrium &eq = tr.eq;
                            const XReadable   &cc = tr.cc;
                            const XReadable   &dc = tr.dc;
                            size_t n = eq->size();
                            for(Equilibrium::ConstIterator it=eq->begin();n>0;--n,++it)
                            {
                                const Species &      sp = (**it).sp;
                                const size_t * const id = sp.indx;
                                const size_t         jj = id[SubLevel];
                                if(cdb.has(sp))
                                {
                                    // conserved species => direct transfer
                                    C[ id[L] ] = cc[jj];
                                }
                                else
                                {
                                    // unbounded species => accumulate
                                    xaccum[jj] << dc[jj];
                                }
                            }
                        }

                        //------------------------------------------------------
                        // collect unbounded
                        //------------------------------------------------------
                        for(const SNode *sn=mine.unbounded.list.head;sn;sn=sn->next)
                        {
                            const Species &      sp = **sn;
                            const size_t * const id = sp.indx;
                            C[ id[L] ] = xaccum[ id[SubLevel] ].sum();
                        }

                        if(false)
                        {

                            for(const SNode *sn = mine.species.head; sn; sn=sn->next)
                            {
                                const Species &sp = **sn;
                                std::cerr << std::setw(15) << real_t( C[ sp.indx[L]]) << " = [" << sp << "]" <<  std::endl;
                            }
                            exit(7);
                        }
                    }
                }




            }


            //! fill concentrations trades and wobbly species
            size_t getUnbalanced(const XReadable &C, const Level L, XMLog &xml)
            {
                Y_XML_SECTION(xml, "getUnbalanced");

                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                size_t             unbalanced = 0;
                const AddressBook &cdb  = mine.conserved.book;
                trades.free();
                wobbly.free();

                //--------------------------------------------------------------
                //
                // loop over LIMITED equilibria
                //
                //--------------------------------------------------------------
                for(const ENode *en=mine.limited.head;en;en=en->next)
                {
                    //----------------------------------------------------------
                    // study equilibrium
                    //----------------------------------------------------------
                    const Equilibrium &eq    = **en;
                    const Trims::Kind  kind  = trms(C,L,eq,cdb);
                    best.free();

                    switch(kind)
                    {
                        case Trims::BadNone: //Y_XMLOG(xml, "(+) " << eq);
                            continue; // next

                        case Trims::BadBoth: Y_XMLOG(xml, "(-) " << eq);
                            ++unbalanced;
                            Y_XMLOG(xml, " |_reac: " << trms.reac.required);
                            Y_XMLOG(xml, " |_prod: " << trms.prod.required);
                            continue; // next

                        case Trims::BadProd:Y_XMLOG(xml, "(>) " << eq);
                            ++unbalanced;
                            Y_XMLOG(xml, " |_prod: " << trms.prod.required);
                            Y_XMLOG(xml, " |_reac: " << trms.reac.limiting);
                            if(trms.reac.limiting.blocking())
                            {
                                Y_XMLOG(xml, " |_blocked...");
                                continue; // next
                            }
                            else
                            {
                                bestEffort(trms.reac.limiting, trms.prod.required);
                                break; // will move
                            }


                        case Trims::BadReac:Y_XMLOG(xml, "(<) " << eq);
                            ++unbalanced;
                            Y_XMLOG(xml, " |_reac: " << trms.reac.required);
                            Y_XMLOG(xml, " |_prod: " << trms.prod.limiting);
                            if(trms.prod.limiting.blocking())
                            {
                                Y_XMLOG(xml, " |_blocked...");
                                continue; // next
                            }
                            else
                            {
                                bestEffort(trms.prod.limiting, trms.reac.required);
                                Coerce(best.xi).neg();
                                break; // will move
                            }
                    }

                    assert( best->size > 0 );
                    Y_XMLOG(xml, " |_best@" << best);

                    //----------------------------------------------------------
                    // setup a new trade
                    //----------------------------------------------------------
                    const xreal_t _0;
                    const size_t  ii = trades.size()+1;
                    XWritable   & cc = mine.transfer(ctrade[ii],SubLevel,C,L);
                    XWritable   & dc = dtrade[ii].ld(_0);

                    //----------------------------------------------------------
                    // update cc
                    //----------------------------------------------------------
                    xadd.free();
                    {
                        const xreal_t xi = best.xi;
                        size_t        n  = eq->size();
                        for(Components::ConstIterator it=eq->begin();n>0;++it,--n)
                        {
                            const Component &cm = **it;
                            const Species   &sp = cm.sp;
                            const size_t     jj = sp.indx[SubLevel];
                            const xreal_t    c0 = cc[jj];
                            const xreal_t    c1 = c0 + xi * cm.xn;
                            const bool       conserved = cdb.has(sp);

                            // careful update
                            if(c0.mantissa<0)
                            {
                                xadd << -c0;
                                cc[jj] = c1;
                            }
                            else
                            {
                                if(conserved)
                                {
                                    // tailored to keep correct
                                    cc[jj] = c1.mantissa>=0 ? c1 : _0;
                                }
                                else
                                {
                                    // whatever
                                    cc[jj] = c1;
                                }

                            }
                        }
                    }

                    //----------------------------------------------------------
                    // force vanishing species from best
                    //----------------------------------------------------------
                    for(const SNode *sn=best->head;sn;sn=sn->next)
                    {
                        cc[ (**sn).indx[SubLevel] ] = _0;
                    }

                    //----------------------------------------------------------
                    // update negative score and dc
                    //----------------------------------------------------------
                    {
                        size_t        n  = eq->size();
                        for(Components::ConstIterator it=eq->begin();n>0;++it,--n)
                        {
                            const Species &sp = (**it).sp;
                            const size_t   jj = sp.indx[SubLevel];
                            const xreal_t  c1 = cc[ jj ];
                            if(c1.mantissa<0) xadd << c1;
                            dc[jj] = c1 - C[ sp.indx[L] ];
                        }
                    }

                    //----------------------------------------------------------
                    // build trade and append it
                    //----------------------------------------------------------
                    {
                        const Trade trade(eq,cc,xadd.sum(),dc);
                        trades << trade;
                    }

                    Y_XMLOG(xml," |_gain: " << trades.tail());
                }

                //--------------------------------------------------------------
                //
                // finalize
                //
                //--------------------------------------------------------------
                assert(0==wobbly.size);
                if(unbalanced>0)
                {
                    for(const SNode *sn=mine.conserved.list.head;sn;sn=sn->next)
                    {
                        const Species &sp = **sn;
                        assert( cdb.has(sp) );
                        if( C[ sp.indx[L] ].mantissa >= 0 ) continue;
                        wobbly << sp;
                    }
                    return unbalanced;
                }
                else
                {
                    return 0;
                }
            }


            //! construct best effort single frontier
            void bestEffort(const SingleFrontier &limiting,
                            const Frontiers      &required)
            {
                assert(required.size>0);        // number of frontiers
                assert(limiting->size>0);       // number of limiting species
                assert(limiting.xi.mantissa>0); // positive limiting extent

                best.free();
                const  Frontier *prev = 0;
                for(const FNode *node=required.head;node;node=node->next)
                {
                    const Frontier &F = **node;
                    switch( Sign::Of(F.xi,limiting.xi) )
                    {
                        case Negative:
                            prev = &F; // almost to this one
                            continue;

                        case __Zero__: // numerical match
                            Coerce(best.xi) = limiting.xi;
                            best << limiting;
                            best << F;
                            return;

                        case Positive:
                            break; // will stop
                    }
                    break; // stop
                }

                if(0!=prev)
                {
                    //----------------------------------------------------------
                    //
                    // found a required before limiting, will solve up to it
                    //
                    //----------------------------------------------------------
                    Coerce(best.xi) = prev->xi;
                    best << *prev;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // no requirement, best partial effort
                    //
                    //----------------------------------------------------------
                    Coerce(best.xi) = limiting.xi;
                    best   << limiting;
                }


            }


            //! check if concentrations need to be injected to obey laws
            /**
             \param G   group to scan
             \param C   concentrations
             \param L   level
             \param xml display
             */
            const Conservation::Law * wasInjected(const Group &G,
                                                  XWritable   &C,
                                                  const Level  L,
                                                  XMLog       &xml)
            {

                //--------------------------------------------------------------
                //
                // initialize all possible fixed in group
                //
                //--------------------------------------------------------------
                if(!initialize(G,C,L,xml)) return 0;

                //--------------------------------------------------------------
                //
                // iterative reduction
                //
                //--------------------------------------------------------------
                const Conservation::Law *law = 0; assert( jail.size() > 0);
                while(jail.size()>0)
                {
                    law = reduce(C,L,xml); assert(0!=law); // record last reduced
                    if(jail.size()<=0) return law;         // early return
                    update(C,L,xml);
                }

                //--------------------------------------------------------------
                //
                // something was modified, return last reduced
                //
                //--------------------------------------------------------------
                assert(0!=law);
                return law;
            }




            //! load C and compute all necessary modified concentrations
            /**
             \return true if must fix some law
             */
            bool initialize(const Group &group,
                            XWritable   &C,
                            const Level  L,
                            XMLog       &xml)
            {
                Y_XML_SECTION_OPT(xml, "initialize", "group='" << group.size << "'");
                jail.free();
                bool mustFix = false;
                for(const LNode *ln=group.head;ln;ln=ln->next)
                {
                    const Conservation::Law &cl = **ln;
                    const size_t             ii = jail.size()+1;
                    XWritable               &cc = mine.transfer(conc[ii],SubLevel,C,L);
                    Fixed                    fx(cc,cl);
                    if( fx.still(C,L,xadd) )
                    {
                        jail << fx;
                        Y_XMLOG(xml, "(+) " << fx);
                        mustFix = true;
                    }
                }
                return mustFix;
            }

            //! find lowest fix and apply it
            /**
             \param C   concentrations to fix
             \param L   level
             \param xml display
             \return last fixed law, to be zeroed!
             */
            const Conservation::Law * reduce(XWritable   &C,
                                             const Level  L,
                                             XMLog       &xml)
            {
                Y_XML_SECTION_OPT(xml,"reduce","size='"<<jail.size()<<"'");

                //------------------------------------------------------
                // sort by decreasing excess
                //------------------------------------------------------
                HeapSort::Call(jail,Fixed::Compare);
                if(xml.verbose)
                {
                    for(size_t i=1;i<jail.size();++i)
                    { Y_XMLOG(xml, "(+) " << jail[i]);     }
                    { Y_XMLOG(xml, "(*) " << jail.tail()); }
                }

                //------------------------------------------------------
                // optimize (and remove) smallest excess
                //------------------------------------------------------
                const Fixed             &fx = jail.tail();
                const Conservation::Law &cl = fx.cl;
                for(const Actor *a=cl->head;a;a=a->next)
                {
                    const Species &      sp = a->sp;
                    const size_t * const id = sp.indx;
                    const size_t         ii = id[SubLevel];
                    const size_t         II = id[L];
                    const xreal_t        c0 = C[ II ];
                    const xreal_t        c1 = fx.cc[ ii ]; assert(c1>=c0);
                    cinj[ii] << (c1-c0);
                    C[II] = c1;
                }
                jail.popTail();
                return &cl;
            }


            //! check who still needs to be fixed
            void update(const XReadable   &C,
                        const Level        L,
                        XMLog             &xml)
            {
                Y_XML_SECTION_OPT(xml,"update","size='"<<jail.size()<<"'");

                //------------------------------------------------------
                // check remaining with new concentration
                //------------------------------------------------------
                for(size_t i=jail.size();i>0;--i)
                {
                    Fixed &fx = jail[i];
                    if( fx.still(C,L,xadd) )
                    {
                        Y_XMLOG(xml, "(>) " << fx);
                    }
                    else
                    {
                        Y_XMLOG(xml, "(<) " << fx);
                        jail.remove(i);
                    }
                }
            }

        };

    }

}

#endif

