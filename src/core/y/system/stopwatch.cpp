
#include "y/system/stopwatch.hpp"

namespace Yttrium
{
    StopWatch:: StopWatch(uint64_t * const pEllapsed) :
    ticks(pEllapsed),
    enter(0!=ticks?WallTime::Ticks():0)
    {

    }

    StopWatch:: ~StopWatch() noexcept
    {
        if(ticks) *ticks += WallTime::Ticks() - enter;
    }


}
