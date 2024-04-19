//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chemical/reactive/equilibria.hpp"
#include "y/stream/xmlog.hpp"
#include "y/container/matrix.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Cluster : public Object, public EList
        {
        public:
            explicit Cluster(const Equilibrium &);
            virtual ~Cluster() noexcept;

            const Cluster & operator*() const noexcept { return *this; }

            friend std::ostream & operator<<(std::ostream &os, const Cluster &cl)
            {
                const EList &el = cl;
                return os << el;
            }

            bool sharesSpeciesWith(const Equilibrium &) const noexcept;
            bool sharesSpeciesWith(const Cluster     &) const noexcept;

            void compile(XMLog &xml);

            const SList       species;
            const Matrix<int> nu;
            const EqFormatter eqfmt;
            const Entities    spfmt;
            Cluster          *next;
            Cluster          *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
        };
    }

}

#endif

