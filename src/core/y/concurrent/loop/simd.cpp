
#include "y/concurrent/loop/simd.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {
            SIMD:: ~SIMD() noexcept {}
            SIMD:: SIMD(const SharedLoop &team) noexcept : loop(team) {}
            SIMD:: SIMD(Loop             *team) noexcept : loop(team) {}

        }
    }

}

