//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chemical/reactive/cluster/combinatorics.hpp"
#include "y/quantized.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Cluster of coupled equilibria and species
        //
        //
        //______________________________________________________________________
        class Cluster : public Quantized, public ClusterCombinatorics
        {
        public:
            typedef CxxListOf<Cluster>                List;  //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! initialize
            /**
             \param eqs   global equilibria
             \param fragment precompiled fragment
             \param topK  shared top-level constants
             */
            explicit Cluster(Equilibria        &eqs,
                             const Fragment    &fragment,
                             const Constants   &topK,
                             XMLog             &xml);

            //! cleaup
            virtual ~Cluster() noexcept;
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________


            const Cluster & operator*() const noexcept { return *this; }

            //! formatted output
            friend std::ostream & operator<<(std::ostream &os, const Cluster &cl);


            //! get all K, in order to validate mixed-equilibria
            void getK(const Real t);

            void viz(OutputStream &fp, const size_t order, const size_t clusterIndex) const;


        public:
            Cluster                    *next;      //!< for list
            Cluster                    *prev;      //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
        };
    }

}

#endif

