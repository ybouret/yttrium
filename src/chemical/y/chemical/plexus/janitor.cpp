
#include "y/chemical/plexus/janitor.hpp"
#include "y/sort/heap.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Janitor:: Janitor(const Cluster &cl) :
        mine(cl),
        init(mine.laws.isValid() ? mine.laws->groups.head : 0),
        rows(init ? mine.laws->maxGroupSize : 0),
        cols(init ? mine.species.size       : 0),
        jail(rows),
        Cnew(rows,cols),
        Cinj(cols),
        xadd(cols),
        zero(0)
        {
        }


        Janitor:: ~Janitor() noexcept
        {
        }


        void Janitor:: prolog() noexcept
        {
            for(size_t i=cols;i>0;--i) Cinj[i].free();
        }

        void Janitor:: epilog(XWritable &dC, const Level L)
        {
            if(0!=init)
            {
                for(const SNode *sn=mine.species.head;sn;sn=sn->next)
                {
                    const Species       &sp   = **sn;
                    const size_t * const indx = sp.indx;
                    dC[ indx[L] ] = Cinj[ indx[SubLevel] ].sum();
                }
            }
            else
            {
                for(const SNode *sn=mine.species.head;sn;sn=sn->next)
                {
                    dC[ (**sn).indx[L] ] = zero;
                }
            }
        }


        void Janitor:: process(XWritable  &C,
                               const Level L,
                               XMLog      &xml)
        {
            Y_XML_SECTION(xml, "Janitor");
            for(const Group *g=init;g;g=g->next)
                process(*g,C,L,xml);
        }

        void  Janitor:: process(const Group &grp,
                                XWritable   &C,
                                const Level  L,
                                XMLog       &xml)
        {
            Y_XML_SECTION_OPT(xml, "Group", "size='" << grp.size << "'");

            //__________________________________________________________________
            //
            //
            // initialize with all broken laws
            //
            //__________________________________________________________________
            jail.free();
            {
                Y_XML_SECTION(xml, "Initialize");
                for(Conservation::LNode *ln=grp.head;ln;ln=ln->next)
                {
                    const Conservation::Law &claw = **ln;
                    Broken                   broken(claw,Cnew[jail.size()+1]);
                    if(broken.still(C,L,xadd))
                    {
                        Y_XMLOG(xml," (+) " << broken);
                        jail << broken;
                    }
                }
            }

            //__________________________________________________________________
            //
            //
            // iterative reduction
            //
            //__________________________________________________________________
            while(jail.size()>0)
            {
                {
                    Y_XML_SECTION_OPT(xml,"Reduction"," broken='" << jail.size() << "'");

                    //__________________________________________________________
                    //
                    // order by increasing gain
                    //__________________________________________________________
                    HeapSort::Call(jail,Broken::Compare);
                    for(size_t i=1;i<jail.size();++i) { Y_XMLOG(xml," (+) " << jail[i]); }

                    //__________________________________________________________
                    //
                    // fixed least broken
                    //__________________________________________________________
                    {
                        const Broken & best = jail.tail();
                        Y_XMLOG(xml," (*) " << best);
                        const Conservation::Law &claw = best.claw;
                        const XReadable         &cfix = best.cfix;
                        for(const Actor *a=claw->head;a;a=a->next)
                        {
                            const size_t   isub = a->sp.indx[SubLevel];
                            const size_t   iusr = a->sp.indx[L];
                            const xreal_t  Cnew = cfix[ isub ];
                            xreal_t       &Cold = C[ iusr ];
                            assert(Cnew>=Cold);
                            Cinj[isub] << (Cnew-Cold); // store  difference
                            Cold = Cnew;               // update concentration
                        }
                    }

                    //__________________________________________________________
                    //
                    // remove it
                    //__________________________________________________________
                    jail.popTail();
                    if(jail.size()<=0) break;
                }


                //__________________________________________________________________
                //
                //
                // then update and keep/drop remaining broken
                //
                //__________________________________________________________________
                {
                    Y_XML_SECTION_OPT(xml,"Upgrading"," broken='" << jail.size() << "'");
                    for(size_t i=jail.size();i>0;--i)
                    {
                        if( jail[i].still(C,L,xadd) )
                        {
                            Y_XMLOG(xml, " (+) " << jail[i]);
                        }
                        else
                        {
                            Y_XMLOG(xml, " (-) " << jail[i]);
                            jail.remove(i);
                        }
                    }
                }
            }

        }

        void Janitor:: display(std::ostream &os, const Library &lib) const
        {
            os << "{" << std::endl;
            if(0!=init)
            {
                for(const SNode *sn = mine.species.head; sn; sn=sn->next)
                {
                    const Species &sp = **sn;
                    lib.pad(os << "d[" << sp << "]",sp) << '=' << Cinj[ sp.indx[SubLevel] ] << std::endl;
                }
            }
            os << "}" << std::endl;
        }
    }

}
