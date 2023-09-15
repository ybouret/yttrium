//! \file

#ifndef Y_ENV_Included
#define Y_ENV_Included 1

#include "y/string.hpp"

namespace Yttrium
{
    //! getting Environment variables
    struct Environment
    {

        static bool Get(String &value, const String &     name); //!< get value from name if exists
        static bool Get(String &value, const char * const name); //!< wrapper

        static bool Flag(const String &     name); //!< true if exists and is 1,true,on but not 0,false,off
        static bool Flag(const char * const name); //!< wrapper

    };
}

#endif
