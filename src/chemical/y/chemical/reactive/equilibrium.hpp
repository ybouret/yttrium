
//! \file

#ifndef Y_Chemical_Equilibrium_Included
#define Y_Chemical_Equilibrium_Included 1

#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Equilibrium : public Components
        {
        public:
            virtual ~Equilibrium() noexcept;
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
        };

    }

}

#endif

