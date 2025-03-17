
//! \file

#ifndef Y_Chemical_ClusterConservations_Included
#define Y_Chemical_ClusterConservations_Included 1

#include "y/chemical/plexus/cluster/topology.hpp"
#include "y/chemical/plexus/conservation/laws.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class ClusterConservations : public ClusterTopology
        {
        public:
            explicit ClusterConservations(XMLog &, const ClusterContent::Pointer &);
            virtual ~ClusterConservations() noexcept;

            const uMatrix            preserved; //!< preserved matrix
            const Conservation::Laws ordinance; //!< matching laws
            const SList              conserved; //!< conserved species
            const SList              unbounded; //!< unbounded species

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterConservations);
        };

    }

}


#endif

