#include "y/chemical/reactive/cluster/constellation.hpp"




namespace Yttrium
{
    namespace Chemical
    {
        ClusterConstellation:: ClusterConstellation(Equilibria        &eqs,
                                                    const Fragment    &fragment,
                                                    const Constants   &topK,
                                                    XMLog             &xml) :
        ClusterCombinatorics(eqs,fragment,topK,xml)
        {

            static const char here[] = "Chemical::Cluster::Constellation";
            Y_XML_SECTION_OPT(xml, here, " eqs=" << size);

            exit(0);

        }
        
        ClusterConstellation:: ~ClusterConstellation() noexcept
        {
        }



    }
}
