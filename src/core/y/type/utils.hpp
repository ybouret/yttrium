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
    //! lhs <- Min(lhs,rhs)
    //__________________________________________________________________________
    template <typename T> inline
    void InSituMin(T &lhs, const T rhs) {
        lhs = Min<T>(lhs,rhs);
    }

    //__________________________________________________________________________
    //
    //! lhs <- Max(lhs,rhs)
    //__________________________________________________________________________
    template <typename T> inline
    void InSituMax(T &lhs, const T rhs) {
        lhs = Max<T>(lhs,rhs);
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

    //__________________________________________________________________________
    //
    //! squared x
    //__________________________________________________________________________
    template <typename T> inline
    T Squared(T x)
    {
        return x*x;
    }

    //__________________________________________________________________________
    //
    //! twice x
    //__________________________________________________________________________
    template <typename T> inline
    T Twice(T x)
    {
        return x+x;
    }

    //__________________________________________________________________________
    //
    //! identity functor
    //__________________________________________________________________________
    template <typename T> inline
    const T &Identity(const T &x) noexcept
    {
        return x;
    }

}

#endif

