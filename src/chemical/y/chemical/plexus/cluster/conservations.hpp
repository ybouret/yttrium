
//! \file

#ifndef Y_Chemical_ClusterConservations_Included
#define Y_Chemical_ClusterConservations_Included 1

#include "y/chemical/plexus/cluster/topology.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef Matrix<unsigned> uMatrix;

        class ClusterConservations : public ClusterTopology
        {
        public:
            explicit ClusterConservations(XMLog &, const ClusterContent::Pointer &);
            virtual ~ClusterConservations() noexcept;

            const uMatrix preserved;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterConservations);
        };

    }

}


#endif

