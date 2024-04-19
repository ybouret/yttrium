
#include "y/chemical/reactive/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Clusters:: Clusters(Equilibria &eqs, XMLog &xml) : CxxListOf<Cluster>()
        {
            build(eqs,xml);
        }

        Clusters:: ~Clusters() noexcept
        {
        }

    }

}
