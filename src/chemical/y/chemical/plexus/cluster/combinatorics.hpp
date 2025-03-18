//! \file

#ifndef Y_Chemical_ClusterCombinatorics_Included
#define Y_Chemical_ClusterCombinatorics_Included 1

#include "y/chemical/plexus/cluster/conservations.hpp"
#include "y/chemical/reactive/equilibria.hpp"
#include "y/container/cxx/array.hpp"

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
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxArray<EList,MemoryModel> ELists; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit ClusterCombinatorics(XMLog                         &xml,
                                          const ClusterContent::Pointer &ptr,
                                          Equilibria                    &eqs,
                                          XWritable                     &tlK); //!< setup
            virtual ~ClusterCombinatorics() noexcept;                                //!< cleanup

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const iMatrix Nu;         //!< full topology
            //const EList   standard;
            //const EList   reacOnly;
            //const EList   prodOnly;
            //const EList   nebulous;
            const ELists  order;     //!< equilibria ranked by order


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterCombinatorics);

        };

    }

}

#endif

