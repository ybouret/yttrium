//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chemical/reactive/equilibria.hpp"
#include "y/chemical/type/constants.hpp"

#include "y/stream/xmlog.hpp"
#include "y/container/matrix.hpp"
#include "y/chemical/type/entity-set.hpp"
#include "y/quantized.hpp"

namespace Yttrium
{
    namespace Chemical
    {




        //______________________________________________________________________
        //
        //
        //
        //! Cluster of coupled species
        //
        //
        //______________________________________________________________________
        class Cluster : public Quantized, public EList
        {
        public:
            typedef Vector<EList,Memory::Dyadic> Blend;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! initialize with first equilibrium and TopLevel constants
            explicit Cluster(const Equilibrium &first,
                             const Constants   &topK);

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


            bool sharesSpeciesWith(const Equilibrium &) const noexcept; //!< check if species are shared
            bool sharesSpeciesWith(const Cluster     &) const noexcept; //!< check if species are shared

            //! one filled, compile all subsequent data
            void compile(Equilibria &eqs, XMLog &xml);

            //! get all K, in order to validate mixed-equilibria
            void getK(const Real t);

        private:
            Constants           sharedK; //!< TopLevel constants
        public:
            const SList         species; //!< SubLevel, primary species
            const Matrix<int>   Nu;      //!< SubLevel topology
            const SpSubSet      spset;   //!< table of species
            const EqSubSet      eqset;   //!< table of equilibria
            const EqFormatter   eqfmt;   //!< helper to display
            const Entities      spfmt;   //!< helper to display
            const Blend         blend;   //!< 
            Cluster            *next;    //!< for list
            Cluster            *prev;    //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            String buildMixedName(const Readable<int> &w) const;
            void buildCombinatorics(Equilibria &eqs,XMLog &xml);
            //void buildConservations();
        };
    }

}

#endif

