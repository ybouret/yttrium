
//! \file

#ifndef Y_Comparison_Included
#define Y_Comparison_Included 1

#include "y/type/signs.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //! Comparison function
    //
    //__________________________________________________________________________
    struct Comparison
    {

        //______________________________________________________________________
        //
        //! C style increasing
        //______________________________________________________________________
        template <typename T> static inline
        int Increasing(const T &lhs, const T &rhs)
        {
            return lhs < rhs ? -1 : ( rhs < lhs ? 1 : 0);
        }

        //______________________________________________________________________
        //
        //! C style decreasing
        //______________________________________________________________________
        template <typename T> static inline
        int Decreasing(const T &lhs, const T &rhs)
        {
            return lhs < rhs ? 1 : ( rhs < lhs ? -1 : 0);
        }

        //______________________________________________________________________
        //
        //! C++ style increasing
        //______________________________________________________________________
        template <typename T> static inline
        SignType CxxIncreasing(const T &lhs, const T &rhs)
        {
            return Sign::Of(lhs,rhs);
        }

        //______________________________________________________________________
        //
        //! C++ style decreasing
        //______________________________________________________________________
        template <typename T> static inline
        SignType CxxDecreasing(const T &lhs, const T &rhs)
        {
            return Sign::Of(rhs,lhs);
        }

    };

}

#endif

