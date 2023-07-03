
//! \file

#ifndef Y_Text_Plural_Included
#define Y_Text_Plural_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //!  Plural forms
    //
    //__________________________________________________________________________
    struct Plural
    {
        static const char *S(const size_t) noexcept; //!< "S" | ""
        static const char *s(const size_t) noexcept; //!< "s" | ""
    };
}

#endif

