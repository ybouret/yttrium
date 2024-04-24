
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
        sharedK(topK),
        maxGroupSize(0)
        {
            Y_XML_SECTION(xml, "Chemical::Clusters" );

            // create batches
            const Batches batches(eqs);

            // create one cluster per batch
            for(const Batch *batch=batches.head;batch;batch=batch->next)
            {
                const Cluster *cl = clusters.pushTail( new Cluster(eqs,*batch,sharedK,xml) );
                Coerce(maxGroupSize) = Max(cl->maxGroupSize,maxGroupSize);
            }
            Y_XMLOG(xml, "  (*) #cluster=" << clusters.size );
            Y_XMLOG(xml, "  (*) conservation max group size : " << maxGroupSize);
            // adjust top-level constants
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
