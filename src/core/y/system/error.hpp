//! \file

#ifndef Y_System_Error_Included
#define Y_System_Error_Included 1

#include "y/config/starting.hpp"
#include "y/check/printf.hpp"

namespace Yttrium
{

    namespace Libc
    {
        //______________________________________________________________________
        //
        //
        //! using strerror
        //
        //______________________________________________________________________
        void FormatError(char  *      buffer,
                         const size_t buflen,
                         const int    err) noexcept;

        //______________________________________________________________________
        //
        //
        //! using strerror + formated message, then abort
        //
        //______________________________________________________________________
        void CriticalError(const int err, const char *fmt,...) Y_PRINTF_CHECK(2,3);
    }


}

#endif

