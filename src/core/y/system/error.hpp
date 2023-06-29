//! \file

#ifndef Y_System_Error_Included
#define Y_System_Error_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Libc
    {
        void FormatError(char  *      buffer,
                         const size_t buflen,
                         const int    err) noexcept;
    }


}

#endif

