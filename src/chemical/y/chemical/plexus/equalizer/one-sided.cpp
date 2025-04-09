
#include "y/chemical/plexus/equalizer/one-sided.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {
            const char * const OneSided:: CallSign = "Equalizer::OneSided";

            OneSided:: OneSided(const Cluster & _cluster,
                                const Banks   & _banks) :
            cluster(_cluster),
            cursors(_banks)
            {

            }

            OneSided:: ~OneSided() noexcept
            {

            }


            static inline
            bool cursorsWerePopulated(Cursors          &cursors,
                                      const Actor *     a,
                                      const XReadable  &C,
                                      const Level       L)
            {
                const xreal_t zero;
                cursors.restart();
                for(;a;a=a->next)
                {
                    const Species &sp = a->sp;
                    const xreal_t  cc = sp(C,L);
                    if(cc<zero) cursors(sp,(-cc)/a->xn);
                }
                return cursors->size>0;
            }


            void OneSided:: fix(XMLog     & xml,
                                XWritable & C0,
                                const Level L0)
            {
                Y_XML_SECTION(xml,"Equalizer::OneSided");
                for(const ENode *en=cluster.onlyProd->head;en;en=en->next)
                {
                    const Equilibrium &E = **en;
                    if(cursorsWerePopulated(cursors,E.prod->head, C0, L0))
                    {
                        Y_XMLOG(xml,E.name << " : " << cursors);
                        (**cursors->tail).zforward(E,C0,L0);
                    }
                }


                for(const ENode *en=cluster.onlyReac->head;en;en=en->next)
                {
                    const Equilibrium &E = **en;
                    if(cursorsWerePopulated(cursors, E.reac->head, C0, L0))
                    {
                        Y_XMLOG(xml,E.name << " : " << cursors);
                        (**cursors->tail).zreverse(E,C0,L0);
                    }
                }

            }

        }

    }

}
