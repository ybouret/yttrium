//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chem/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Cluster
        {
        public:
            explicit Cluster();
            virtual ~Cluster() noexcept;

            Equilibria &eqs;
            Cluster    *next;
            Cluster    *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);

        };
    }
}

#endif
