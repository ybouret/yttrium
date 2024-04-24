
//! \file

#ifndef Y_Chemical_Cluster_Conservations_Included
#define Y_Chemical_Cluster_Conservations_Included 1

#include "y/chemical/reactive/cluster/topology.hpp"
#include "y/chemical/reactive/conservation/groups.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Conmpute conservations from topology
        //
        //
        //______________________________________________________________________
        class ClusterConservations : public ClusterTopology
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from fragment
            explicit ClusterConservations(const Fragment    &fragment,
                                          const Constants   &topK,
                                          XMLog             &xml);

            //! cleanup
            virtual ~ClusterConservations() noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Matrix<unsigned>     Qm;               //!< conservation matrix
            const Conservation::Laws   laws;             //!< corresponding conservation laws
            const Conservation::Groups groups;           //!< independent groups of dependents laws
            const size_t               maxGroupSize;     //!< maximum 
            const AddressBook          conserved;        //!< conserved species database
            const AddressBook          unbounded;        //!< unbounded species database
            const SList                conservedSpecies; //!< conserved species list
            const SList                unboundedSpecies; //!< unbounded species list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterConservations);
        };
    }

}

#endif

