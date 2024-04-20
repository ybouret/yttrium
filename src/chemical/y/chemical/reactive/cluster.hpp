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


        class Cluster : public Quantized, public EList
        {
        public:
            explicit Cluster(const Equilibrium &first,
                             const Constants   &topK);

            virtual ~Cluster() noexcept;

            const Cluster & operator*() const noexcept { return *this; }

            friend std::ostream & operator<<(std::ostream &os, const Cluster &cl)
            {
                const EList &el = cl;
                return os << el;
            }

            bool sharesSpeciesWith(const Equilibrium &) const noexcept;
            bool sharesSpeciesWith(const Cluster     &) const noexcept;

            void compile(Equilibria &eqs, XMLog &xml);
            void computeK(const Real t);

        private:
            Constants           sharedK;
        public:
            const SList         species;
            const Matrix<int>   Nu;
            const SpSubSet      spset;
            const EqSubSet      eqset;
            const EqFormatter   eqfmt;
            const Entities      spfmt;
            Cluster            *next;
            Cluster            *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            String buildMixedName(const Readable<int> &w) const;
            void buildCombinatorics(Equilibria &eqs,XMLog &xml);
            //void buildConservations();
        };
    }

}

#endif

