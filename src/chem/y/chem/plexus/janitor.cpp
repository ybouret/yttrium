
#include "y/chem/plexus/janitor.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Janitor:: ~Janitor() noexcept
        {
        }

        Janitor:: Janitor()
        {

        }


        void Janitor:: process(const Cluster   &cluster,
                               Writable<xreal> &Corg)
        {

            const EqRepo &repo = cluster.army->definite;
            for(const EqNode *en=repo.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                
            }


        }
    }

}
