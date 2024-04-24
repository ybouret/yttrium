
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
        maxSPC(0),
        maxCPG(0)
        {
            Y_XML_SECTION(xml, "Chemical::Clusters" );

            //__________________________________________________________________
            //
            // create batches
            //
            //__________________________________________________________________
            const Batches batches(eqs);

            //__________________________________________________________________
            //
            // create one cluster per batch
            //__________________________________________________________________
            for(const Batch *batch=batches.head;batch;batch=batch->next)
            {
                const Cluster *cl = clusters.pushTail( new Cluster(eqs,*batch,sharedK,xml) );
                for(const Conservation::Group *grp=cl->groups->head;grp;grp=grp->next)
                {
                    const Conservation::Group &g = *grp;
                    Coerce(maxCPG) = Max(g->size,maxCPG);
                    Coerce(groups) << g;
                }
                Coerce(maxSPC) = Max(cl->species.size,maxSPC);
            }

            //__________________________________________________________________
            //
            // finalize
            //__________________________________________________________________
            {
                Y_XML_SECTION(xml, "Summary");
                Y_XMLOG(xml, "  (*) #Cluster                   : " << clusters.size);
                Y_XMLOG(xml, "  (*) Max Species Per Cluster    : " << maxSPC);
                Y_XMLOG(xml, "  (*) #Conservation Group        : " << groups.size);
                for(const GNode *gn=groups.head;gn;gn=gn->next)
                {
                    Y_XMLOG(xml,"   |-" << **gn);
                }
                Y_XMLOG(xml, "  (*) Max Conservation Per Group : " << maxCPG);
            }

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

        const Readable<xreal_t> & Clusters :: K(const real_t t)
        {
            for(Cluster *cl=clusters.head;cl;cl=cl->next)
                cl->getK(t);
            return *sharedK;
        }


    }

}
