
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

            //! sanitize  equilibria
            void sanitize(XWritable &C, const Level L, XMLog &xml);



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
             enter when sanitized and possibly lawz
             */
            void equalize(XWritable &C, const Level L, XMLog &xml);

            //! enforce law and store delta in injected
            void zeroLaws(XWritable  &C, const Level L, XMLog      &xml);

            //! optimize found trades from getUnbalanced()
            void   optimizeTrade(XWritable &C, const Level L, XMLog &xml);

            //! fill concentrations trades and wobbly species
            size_t getUnbalanced(const XReadable &C, const Level L, XMLog &xml);


            //! construct best effort single frontier
            void buildFrontier(const SingleFrontier &limiting,
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

