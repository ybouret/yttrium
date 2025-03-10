
#include "y/chemical/type/latchable.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Latchable:: Latchable() noexcept : latched(false)
        {
        }

        Latchable:: ~Latchable() noexcept
        {
        }

        void Latchable:: latch() noexcept
        {
            Coerce(latched) = true;
        }

    }

}
