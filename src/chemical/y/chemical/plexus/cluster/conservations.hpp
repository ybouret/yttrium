
//! \file

#ifndef Y_Chemical_ClusterConservations_Included
#define Y_Chemical_ClusterConservations_Included 1

#include "y/chemical/plexus/cluster/topology.hpp"
#include "y/chemical/plexus/conservation/laws.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Create conservations from topology
        //
        //
        //______________________________________________________________________
        class ClusterConservations : public ClusterTopology
        {
        public:
            //typedef AutoPtr<const Conservation::Canons> CanonsPointer;
            explicit ClusterConservations(XMLog &, const ClusterContent::Pointer &); //!< setup
            virtual ~ClusterConservations() noexcept;                                //!< setup

            const uMatrix            preserved; //!< preserved matrix
            const Conservation::Laws ordinance; //!< matching laws
            const SList             &conserved; //!< conserved species
            const SList              unbounded; //!< unbounded species
            const AddressBook        wandering; //!< dictionary of wandering species

            bool   areAllUnbounded(const Actors &) const noexcept; //!< are all actors unbounded ?
            bool   gotAnyConserved(const Actors &) const noexcept; //!< is at least one actor conserved ?

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterConservations);

        };

    }

}


#endif

