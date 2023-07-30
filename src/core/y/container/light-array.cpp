
#include "y/container/light-array.hpp"

namespace Yttrium
{

    namespace Core
    {
        LightArray:: ~LightArray() noexcept
        {
            Coerce(room) = 0;
        }

        LightArray:: LightArray(const size_t n) noexcept : room(n) {}

        const char * const LightArray:: CallSign = "LightArray";

    }

}

