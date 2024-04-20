
#include "y/chemical/reactive/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Clusters:: Clusters(Equilibria &eqs, const Readable<XReal> &topK, XMLog &xml) : CxxListOf<Cluster>()
        {
            build(eqs,topK,xml);
        }

        Clusters:: ~Clusters() noexcept
        {
        }

    }

}
