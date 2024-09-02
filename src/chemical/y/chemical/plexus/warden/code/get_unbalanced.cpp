
#include "y/chemical/plexus/warden.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        size_t Warden:: getUnbalanced(const XReadable &C, const Level L, XMLog &xml)
        {
            Y_XML_SECTION(xml, "getUnbalanced");

            //--------------------------------------------------------------
            //
            // initialize
            //
            //--------------------------------------------------------------
            size_t             unbalanced = 0;
            const AddressBook &cdb        = mine.conserved.book;
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
                            buildFrontier(trms.reac.limiting, trms.prod.required);
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
                            buildFrontier(trms.prod.limiting, trms.reac.required);
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
                assert(wobbly.size>0);
                return unbalanced;
            }
            else
            {
                return 0;
            }
        }
    }

}


