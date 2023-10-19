
//! \file

#ifndef Y_Chemical_Reactive_Army_Included
#define Y_Chemical_Reactive_Army_Included 1

#include "y/chem/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Army : public Object
        {
        public:
            explicit Army() noexcept;
            virtual ~Army() noexcept;
            
            const EqRepo prodOnly;
            const EqRepo reacOnly;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Army);
        };
    }

}

#endif
