
//! \file

#ifndef Y_Chemical_Cluster_Constellation_Included
#define Y_Chemical_Cluster_Constellation_Included 1

#include "y/chemical/reactive/cluster/combinatorics.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Compute all possible equilibria from topology
        //
        //
        //______________________________________________________________________
        class ClusterConstellation : public ClusterCombinatorics
        {
        public:
            explicit ClusterConstellation(Equilibria        &eqs,
                                          const Fragment    &fragment,
                                          const Constants   &topK,
                                          XMLog             &xml);
            virtual ~ClusterConstellation() noexcept;

            const AddressBook roamingBoth; //!< reactants and products are ALL roaming
            const AddressBook roamingReac; //!< reactants are ALL roaming
            const AddressBook roamingProd; //!< products  are ALL roaming
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterConstellation);
        };

    }

}

#endif
