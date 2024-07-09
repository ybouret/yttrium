//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chemical/reactive/equilibria.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Cluster : public Proxy<const EList>, public Party
        {
        public:
            typedef CxxListOf<Cluster> List;

            virtual ~Cluster() noexcept;
            explicit Cluster(const Equilibrium &first);

            bool accept(const Equilibrium &) const noexcept;
            bool accept(const Cluster     &) const noexcept;
            void compile(XMLog &xml);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            virtual ConstInterface & surrogate() const noexcept;
            EList       eqs;

        public:
            const SList species;
            Cluster *   next;
            Cluster *   prev;
        };
    }

}


#endif

