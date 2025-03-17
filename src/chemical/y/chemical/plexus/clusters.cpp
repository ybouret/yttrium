#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/plexus/cluster/builder.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Y_PROXY_IMPL(Clusters,my)

        Clusters:: ~Clusters() noexcept
        {
            
        }

        
        Clusters:: Clusters(XMLog &xml, Equilibria &eqs) :
        my(),
        tlK(),
        K(tlK)
        {
            Y_XML_SECTION_OPT(xml, "Clusters", "#eqs=" << eqs->size());
            ClusterBuilder cls(xml,eqs);
            for(const ClusterKnot *cl=cls->head;cl;cl=cl->next)
            {
                //std::cerr << "cluster: " << *cl << std::endl;
                const ClusterContent::Pointer clc( &Coerce(**cl) );
                my.pushTail( new Cluster(xml,clc,eqs,tlK) );
                //const ClusterCombinatorics    ct(xml,clc,eqs,tlK);
            }
        }



    }

}

