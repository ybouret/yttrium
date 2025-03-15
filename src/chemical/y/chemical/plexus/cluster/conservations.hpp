
//! \file

#ifndef Y_Chemical_ClusterConservations_Included
#define Y_Chemical_ClusterConservations_Included 1

#include "y/chemical/plexus/cluster/topology.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef Matrix<unsigned> uMatrix;

        class Conservation : public Actors
        {
        public:
            explicit Conservation() noexcept;
            virtual ~Conservation() noexcept;

            Conservation *next;
            Conservation *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Conservation);
        };

        class ClusterConservations : public ClusterTopology
        {
        public:
            explicit ClusterConservations(XMLog &, const ClusterContent::Pointer &);
            virtual ~ClusterConservations() noexcept;

            const uMatrix preserved; //!< preserved matrix
            const SList   conserved; //!< conserved species
            const SList   unbounded; //!< unbounded species

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterConservations);
        };

    }

}


#endif

