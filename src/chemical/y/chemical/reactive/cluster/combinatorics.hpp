
//! \file

#ifndef Y_Chemical_Cluster_Combinatorics_Included
#define Y_Chemical_Cluster_Combinatorics_Included 1

#include "y/chemical/reactive/cluster/conservations.hpp"

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
        class ClusterCombinatorics : public ClusterConservations
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Vector<EList,Memory::Dyadic>      Blend; //!< alias to hold blends of equilibria per order

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            explicit ClusterCombinatorics(Equilibria        &eqs,
                                          const Fragment    &fragment,
                                          const Constants   &topK,
                                          XMLog             &xml);
            //! cleanup
            virtual ~ClusterCombinatorics() noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________

            //! lists of equilibria per order, always blend[1]
            const Blend       blend;
           
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterCombinatorics);
            String buildMixedName(const Readable<int> &w) const;
        };
    }

}

#endif
