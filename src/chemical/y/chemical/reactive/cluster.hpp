//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chemical/reactive/cluster/topology.hpp"
#include "y/chemical/type/constants.hpp"
#include "y/chemical/reactive/conservation/groups.hpp"
#include "y/stream/xmlog.hpp"
#include "y/quantized.hpp"

namespace Yttrium
{
    namespace Chemical
    {

       

        class ClusterConservations : public ClusterTopology
        {
        public:
            explicit ClusterConservations(const Fragment    &fragment,
                                          const Constants   &topK,
                                          XMLog             &xml);

            virtual ~ClusterConservations() noexcept;

            const Matrix<unsigned>    Qm;
            const Conservation::Laws  claws;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterConservations);
        };

        //______________________________________________________________________
        //
        //
        //
        //! Cluster of coupled equilibria and species
        //
        //
        //______________________________________________________________________
        class Cluster : public Quantized, public EList
        {
        public:
            typedef CxxListOf<Cluster>                List;  //!< alias
            typedef Vector<EList,Memory::Dyadic>      Blend; //!< alias to hold blends of equilibria per order

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! initialize
            /**
             \param eqs   global equilibria
             \param batch precompiled batch
             \param topK  shared top-level constants
             */
            explicit Cluster(Equilibria        &eqs,
                             const Fragment    &batch,
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

        private:
            Constants                   sharedK; //!< TopLevel constants
        public:
            const SList                 species;   //!< SubLevel, primary species
            const Matrix<int>           Nu;        //!< SubLevel topology
            const Matrix<unsigned>      Qm;        //!< SubLevel conservation in Nu ortho-space
            const SpSubSet              spset;     //!< table of SubLevel species
            const EqSubSet              eqset;     //!< table of SubLevel equilibria
            const EqFormatter           eqfmt;     //!< helper to display
            const Entities              spfmt;     //!< helper to display
            const Conservation::Laws    claws;
            const Conservation::Groups  cgrps;     //!< conservation law operating groups
           // const AddressBook           unbounded; //!< unbounded species
           //const AddressBook           conserved; //!< conserved species
            const Blend                 blend;     //!< order 1...blend.size() equilibria
            Cluster                    *next;      //!< for list
            Cluster                    *prev;      //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            String buildMixedName(const Readable<int> &w) const;
            void   buildCombinatorics(Equilibria &eqs,XMLog &xml);
            void   buildConservations(XMLog &xml);
        };
    }

}

#endif

