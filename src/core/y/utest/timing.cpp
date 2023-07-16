
#include "y/utest/timing.hpp"
#include <cmath>

namespace Yttrium
{

    Timing:: ~Timing() noexcept
    {
    }

    Timing:: Timing() : WallTime(), ticks(0), cycle(0)
    {
    }

    void Timing:: reset() noexcept
    {
        ticks = 0;
        cycle = 0;
    }

    uint64_t Timing:: speed() const noexcept
    {
        const long double numer = cycle;
        const long double denom = (*this)(ticks);
        return static_cast<uint64_t>( floorl(numer/denom+0.5L) );
    }

}

