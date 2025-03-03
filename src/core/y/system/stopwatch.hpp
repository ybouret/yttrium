
//! \file

#ifndef Y_System_StopWatch_Included
#define Y_System_StopWatch_Included 1

#include "y/system/wtime.hpp"

namespace Yttrium
{

    class StopWatch
    {
    public:
        StopWatch(uint64_t * const pEllapsed);
        ~StopWatch() noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(StopWatch);
        uint64_t * const ticks;
        const uint64_t   enter;
    };

}

#endif

