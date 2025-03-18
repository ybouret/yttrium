
#include "y/chemical/plexus/cluster/topology.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        ClusterTopology:: ~ClusterTopology() noexcept
        {
        }

        ClusterTopology:: ClusterTopology(XMLog &xml,const ClusterContent::Pointer &ptr) :
        ClusterContent::Pointer(ptr),
        N(ptr->equilibria->size),
        M(ptr->species->size),
        topology(N,M),
        topologyT(M,N)
        {
            Y_XML_SECTION_OPT(xml, "ClusterTopology", ptr->equilibria);

            for(const ENode *en = (*this)->equilibria->head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                eq.fillTopology( eq(Coerce(topology),SubLevel), SubLevel);
            }
            if(N!=MKL::Rank::Of(topology))
                throw Specific::Exception("ClusterTopology", "bad rank");
            
            Coerce(topologyT).assign(TransposeOf,topology);
            Y_XMLOG(xml, "topology=" << topology);
        }
    }

}

