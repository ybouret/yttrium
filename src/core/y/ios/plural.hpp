
//! \file

#ifndef Y_IOS_Plural_Included
#define Y_IOS_Plural_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    struct Plural
    {
        static const char *S(const size_t) noexcept;
        static const char *s(const size_t) noexcept;

    };
}

#endif

