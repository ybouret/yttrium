
//! \file

#ifndef Y_System_StopWatch_Included
#define Y_System_StopWatch_Included 1

#include "y/system/wtime.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! local stopwatch to increase optional ticks
    //
    //
    //__________________________________________________________________________
    class StopWatch
    {
    public:
        StopWatch(uint64_t * const pEllapsed); //!< initialize
        ~StopWatch() noexcept;                 //!< update if necessary

    private:
        Y_DISABLE_COPY_AND_ASSIGN(StopWatch);
        uint64_t * const ticks;
        const uint64_t   enter;
    };

    //! create the stopwatch name
#define Y_StopWatch__(X,Y) X##Y

    //! instantiate the stopwatch name
#define Y_StopWatch_(HOST,ID) volatile Yttrium::StopWatch Y_StopWatch__(__stopwatch,ID)(HOST)

    //! use a local stopwatch tp monitor scoped code
#define Y_StopWatch(HOST) Y_StopWatch_(HOST,__LINE__)
}

#endif

