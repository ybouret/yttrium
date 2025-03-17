

//! \file

#ifndef Y_Chemical_ClusterCombinatorics_Included
#define Y_Chemical_ClusterCombinatorics_Included 1

#include "y/chemical/plexus/cluster/conservations.hpp"
#include "y/chemical/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Create combinatorics from topology and conservations
        //
        //
        //______________________________________________________________________
        class ClusterCombinatorics : public ClusterConservations
        {
        public:
            explicit ClusterCombinatorics(XMLog                         &xml,
                                          const ClusterContent::Pointer &ptr,
                                          Equilibria                    &eqs,
                                          XWritable                     &tlK); //!< setup
            virtual ~ClusterCombinatorics() noexcept;                                //!< cleanup

            const size_t maxOrder;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterCombinatorics);

        };

    }

}

#endif

