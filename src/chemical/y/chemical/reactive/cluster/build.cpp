
#include "y/chemical/reactive/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {
#if 0

        void Clusters:: build(Equilibria &eqs, XMLog &xml)
        {
            //------------------------------------------------------------------
            //
            // creating raw clusters
            //
            //------------------------------------------------------------------
            Y_XML_SECTION_OPT(xml,"Clusters"," totalEqs=" << eqs->size() );
            size_t                    n = eqs->size();
            Equilibria::ConstIterator i = eqs->begin();
            while(n-- > 0)
            {
                const Equilibrium &eq = **(i++);
                for(Cluster *cl=head;cl;cl=cl->next)
                {
                    if(cl->sharesSpeciesWith(eq))
                    {
                        (*cl) << eq;
                        merge();
                        goto DONE;
                    }
                }
                pushTail( new Cluster(eq,sharedK) );
            DONE:
                continue;
            }

            //------------------------------------------------------------------
            //
            //
            // compiling each cluster
            //
            //------------------------------------------------------------------
            for(Cluster *cl=head;cl;cl=cl->next)
                cl->compile(eqs,xml);
        }

        void Clusters:: merge() noexcept
        {
             CxxListOf<Cluster> store;
            while(size>0)
            {
                AutoPtr<Cluster> tmp = popHead();
                for(Cluster *lhs=store.head;lhs;lhs=lhs->next)
                {
                    Cluster &rhs = *tmp;
                    if(lhs->sharesSpeciesWith(rhs))
                    {
                        lhs->mergeTail(rhs);
                        goto DONE;
                    }
                }
                store.pushTail(tmp.yield());
            DONE:
                continue;
            }
            swapWith(store);
        }
#endif

    }

}

