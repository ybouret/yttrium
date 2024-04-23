
//! \file

#ifndef Y_Chemical_Cluster_Conservations_Included
#define Y_Chemical_Cluster_Conservations_Included 1

#include "y/chemical/reactive/cluster/topology.hpp"
#include "y/chemical/reactive/conservation/groups.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class ClusterConservations : public ClusterTopology
        {
        public:
            explicit ClusterConservations(const Fragment    &fragment,
                                          const Constants   &topK,
                                          XMLog             &xml);

            virtual ~ClusterConservations() noexcept;

            const Matrix<unsigned>     Qm;
            const Conservation::Laws   laws;
            const Conservation::Groups groups;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterConservations);
        };
    }

}

#endif

