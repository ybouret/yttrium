

//! \file


#ifndef Y_Chemical_Latchable_Included
#define Y_Chemical_Latchable_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Latchable
        {
        public:

            explicit Latchable() noexcept;
            virtual ~Latchable() noexcept;
            void     latch() noexcept;
            
            bool latched;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Latchable);
        };
    }

}

#endif
