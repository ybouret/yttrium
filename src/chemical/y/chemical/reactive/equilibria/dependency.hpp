//! \file

#ifndef Y_Chemical_Eqs_Dependy_Included
#define Y_Chemical_Eqs_Dependy_Included 1

#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Dependency
        {
        public:
            explicit Dependency();
            virtual ~Dependency() noexcept;

            EBank bank;
            ERepo repo;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dependency);
        };

    }

}


#endif
