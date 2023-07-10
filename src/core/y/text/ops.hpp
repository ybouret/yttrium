
//! \file

#ifndef Y_Text_Ops_Included
#define Y_Text_Ops_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    struct TextOps
    {
        static char *CopyMessage(char *buffer, const size_t buflen, const char *msg) noexcept;
        static char *TrimInvalid(char *buffer, const size_t buflen, const char *bad, const size_t num) noexcept;
    };

}

#endif

