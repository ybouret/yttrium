
//! \file

#ifndef Y_System_Seed_Included
#define Y_System_Seed_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! computing a random seed from pid and ticks
    //
    //
    //__________________________________________________________________________
    struct SystemSeed
    {
        static uint32_t Get(); //!< from system info
    };
}

#endif
