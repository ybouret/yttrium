//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chemical/reactive/cluster/constellation.hpp"
#include "y/quantized.hpp"
#include "y/container/cxx/array.hpp"

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
        class Cluster : public Quantized, public ClusterConstellation
        {
        public:
            typedef CxxListOf<Cluster>         List;     //!< alias
            typedef CxxArray<bool,MemoryModel> Booleans; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! initialize
            /**
             \param eqs      global equilibria
             \param fragment precompiled fragment
             \param topK     shared top-level constants
             \param xml      for output
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
            const Cluster & operator*() const noexcept { return *this; } //!< helper to display

            //! formatted output
            friend std::ostream & operator<<(std::ostream &os, const Cluster &cl);

            //! get all K, in order to validate mixed-equilibria
            void getK(const real_t t);

            //! emit GraphViz Code
            void viz(OutputStream &fp, const size_t order, const size_t clusterIndex) const;

            //! limited[ sp.indx[SubLevel] ]
            bool isLimited(const Species &sp) const noexcept;

            //! equalize concentrations
            /**
             \param target     target concentrations
             \param tgtlvl     target level
             \param components components to equalize
             \param cursor     extent to apply
             \param zeroed     enforcing zeroed concentrations
             \param source     source concentrations
             \param srclvl     source level
             \return positive gain
             */
            xreal_t equalized(XWritable       &target,
                              const Level       tgtlvl,
                              const Components &components,
                              const xreal_t     cursor,
                              const SNode     *zeroed,
                              const XReadable &source,
                              const Level      srclvl,
                              XAdd            &xadd) const;

        public:
            const Booleans    limited;   //!< sub-level limited species
            Cluster          *next;      //!< for list
            Cluster          *prev;      //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
        };
    }

}

#endif

