//! \file

#ifndef Y_ENV_Included
#define Y_ENV_Included 1

#include "y/string.hpp"

namespace Yttrium
{
    struct Environment
    {
        static bool Get(String &value, const String &     name);
        static bool Get(String &value, const char * const name);

        static bool Flag(const String &     name);
        static bool Flag(const char * const name);

    };
}

#endif
