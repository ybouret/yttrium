
#include "y/chemical/reactive/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Clusters:: Clusters(Equilibria &eqs, const Constants &topK, XMLog &xml) :
        CxxListOf<Cluster>(),
        sharedK(topK)
        {
            build(eqs,xml);
            sharedK->adjust(eqs->size(),0);
        }

        Clusters:: ~Clusters() noexcept
        {
        }

        const Readable<XReal> & Clusters :: K(const Real t)
        {
            for(Cluster *cl=head;cl;cl=cl->next)
                cl->getK(t);
            return *sharedK;
        }


    }

}
