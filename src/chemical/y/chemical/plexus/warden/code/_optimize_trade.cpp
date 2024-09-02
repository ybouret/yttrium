#include "y/chemical/plexus/warden.hpp"
#include "y/sort/heap.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void   Warden:: optimizeTrade(XWritable &C, const Level L, XMLog &xml)
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
                    
                }
            }




        }

    }

}

