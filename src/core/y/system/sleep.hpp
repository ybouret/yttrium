
//! \file

#ifndef Y_System_Sleep_Included
#define Y_System_Sleep_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    struct SystemSleep
    {
        static const unsigned MaxMilliSeconds = 1000;
        static void For(const unsigned ms);
    };
}

#endif

