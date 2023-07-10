//! \file

#ifndef Y_Type_Utils_Included
#define Y_Type_Utils_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //! Min(lhs,rhs)
    //__________________________________________________________________________
    template <typename T> inline
    T Min(const T lhs, const T rhs)
    {
        return lhs < rhs ? lhs : rhs;
    }

    //__________________________________________________________________________
    //
    //! Max(lhs,rhs)
    //__________________________________________________________________________
    template <typename T> inline
    T Max(const T lhs, const T rhs)
    {
        return lhs < rhs ? rhs : lhs;
    }

    //__________________________________________________________________________
    //
    //!  Min(Max(lo,x),hi);
    //__________________________________________________________________________
    template <typename T> inline
    T Clamp(const T lo, const T x, const T hi)
    {
        return Min(Max(lo,x),hi);
    }

    template <typename T> inline
    T Squared(T x)
    {
        return x*x;
    }

}

#endif

