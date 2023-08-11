//! \file

#ifndef Y_Calculus_IntegerSqrt_Included
#define Y_Calculus_IntegerSqrt_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    //! integer square root
    template <typename T> inline
    T IntegerSquareRoot(const T s) noexcept
    {
        if(s<=1) return s;
        T x0 = s >> 1 ;
        while(true) {
            const T x1 = (x0+s/x0)>>1;
            if(x1>=x0) break;
            x0 = x1;
        }
        return x0;

    }

}

#endif


