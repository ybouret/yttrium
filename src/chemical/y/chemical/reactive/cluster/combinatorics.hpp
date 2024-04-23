
//! \file

#ifndef Y_Chemical_Cluster_Combinatorics_Included
#define Y_Chemical_Cluster_Combinatorics_Included 1

#include "y/chemical/reactive/cluster/conservations.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class ClusterCombinatorics : public ClusterConservations
        {
        public:
            typedef Vector<EList,Memory::Dyadic>      Blend; //!< alias to hold blends of equilibria per order

            explicit ClusterCombinatorics(Equilibria        &eqs,
                                          const Fragment    &fragment,
                                          const Constants   &topK,
                                          XMLog             &xml);

            virtual ~ClusterCombinatorics() noexcept;


            const Blend blend;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterCombinatorics);
            String buildMixedName(const Readable<int> &w) const;

        };
    }

}

#endif
