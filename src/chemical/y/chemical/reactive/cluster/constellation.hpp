
//! \file

#ifndef Y_Chemical_Cluster_Constellation_Included
#define Y_Chemical_Cluster_Constellation_Included 1

#include "y/chemical/reactive/cluster/combinatorics.hpp"
#include "y/chemical/plexus/controller.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        //______________________________________________________________________
        //
        //
        //
        //! Compute controlling from all possible equilibria
        //
        //
        //______________________________________________________________________
        class ClusterConstellation : public ClusterCombinatorics
        {
        public:
            typedef Matrix<bool,MemoryModel> BMatrix;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! create controllers from combinatorics
            explicit ClusterConstellation(Equilibria        &eqs,
                                          const Fragment    &fragment,
                                          const Constants   &topK,
                                          XMLog             &xml);

            //! cleanup
            virtual ~ClusterConstellation() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            size_t maxSimultaneous() const noexcept; //!< studied together max


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const EList       hasOnlyReac; //!< equilibria with only reac
            const EList       hasOnlyProd; //!< equilibria with only prod
            const Controllers controllers; //!< controlling equilibria
            const BMatrix     cooperative; //!< cooperative controlles

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterConstellation);
        };

    }

}

#endif
