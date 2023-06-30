//! \file

#ifndef Y_Type_Utils_Included
#define Y_Type_Utils_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    template <typename T> inline
    T Min(const T lhs, const T rhs)
    {
        return lhs < rhs ? lhs : rhs;
    }

    template <typename T> inline
    T Max(const T lhs, const T rhs)
    {
        return lhs < rhs ? rhs : lhs;
    }

    template <typename T> inline
    T Clamp(const T lo, const T x, const T hi)
    {
        return Min(Max(lo,x),hi);
    }

    size_t LengthOf(const char *s) noexcept;

}

#endif

