
//! \file

#ifndef Y_System_Sleep_Included
#define Y_System_Sleep_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //! sleep by suspended activity
    //
    //__________________________________________________________________________
    struct SystemSleep
    {
        static const unsigned MaxMilliSeconds = 1000; //!< limit
        static void For(const unsigned ms);           //!< call system API
    };
}

#endif

