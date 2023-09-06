//! \file

#ifndef Y_Calculus_IPower_Included
#define Y_Calculus_IPower_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    template <typename T>
    T ipower(const T x, size_t n) {
        switch(n)
        {
            case 0: return T(1);
            case 1: return x;
            default:
                break;
        }
        assert(n>=1);
        T res = x;
        while(--n > 0)
            res *= x;
        return res;
    }
}

#endif
