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
            typedef CxxArray<EList,MemoryModel>                ELists; //!< alias
            
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
            // Methods
            //
            //__________________________________________________________________

            //! find where equilibria is
            Characteristic characterize(const Equilibrium &) const noexcept;


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const iMatrix  iNu;      //!< full topology, integer
            const XMatrix  xNu;      //!< full topology, xreal_t
            const ELists   order;    //!< equilibria ranked by order
            const RunEList definite; //!< list of definite
            const RunEList onlyProd; //!< list of onlyProd
            const RunEList onlyReac; //!< list of onlyReac
            const RunEList nebulous; //!< list of nebulous

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterCombinatorics);
            void createCombinations(XMLog      &xml,
                                    Equilibria &eqs,
                                    XWritable  &tlK);
            void setClusterTopology(XMLog &xml);
            void makeCharacteristic(XMLog &xml);

        };

    }

}

#endif

