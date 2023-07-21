
//! \file

#ifndef Y_String_Included
#define Y_String_Included 1

#include "y/string/string.hpp"
#include "y/check/printf.hpp"

namespace Yttrium
{
    typedef Core::String<char> String;
    String FormatString(const char *fmt,...) Y_PRINTF_CHECK(1,2);
}

#endif

