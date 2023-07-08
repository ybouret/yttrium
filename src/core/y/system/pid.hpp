//! \file

#ifndef Y_System_PID_Included
#define Y_System_PID_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    struct ProcessId
    {
        static uint64_t Get() noexcept;
    };
}

#endif
