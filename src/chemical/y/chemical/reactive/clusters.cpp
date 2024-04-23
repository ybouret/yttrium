
#include "y/chemical/reactive/clusters.hpp"
#include "y/chemical/reactive/equilibria/batches.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Clusters:: Clusters(Equilibria      &eqs,
                            const Constants &topK,
                            XMLog           &xml) :
        clusters(),
        sharedK(topK)
        {
            Y_XML_SECTION(xml, "Chemical::Clusters" );
            const Batches batches(eqs);
            for(const Batch *batch=batches.head;batch;batch=batch->next)
                clusters.pushTail( new Cluster(eqs,*batch,sharedK,xml) );

            sharedK->adjust(eqs->size(),0);
        }

        Clusters::ConstInterface & Clusters:: surrogate() const noexcept
        {
            return clusters;
        }


        Clusters:: ~Clusters() noexcept
        {
        }

        const Readable<XReal> & Clusters :: K(const Real t)
        {
            for(Cluster *cl=clusters.head;cl;cl=cl->next)
                cl->getK(t);
            return *sharedK;
        }


    }

}
