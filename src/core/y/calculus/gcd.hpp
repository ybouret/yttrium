
//! \file
#ifndef Y_Calculus_GCD_INCLUDED
#define Y_Calculus_GCD_INCLUDED 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Core
    {
        template <typename T> inline
        T GreatestCommonDivisor(T a, T b) noexcept
        {
            assert(a>0);
            assert(b>0);
            if(a<b)
            {
                const T t = a;
                a = b;
                b = t;
            }
            assert(a>=b);
            while(b>0)
            {
                const T r = a%b;
                a = b;
                b = r;
            }

            return a;
        }
    }

    template <typename T> inline
    T GreatestCommonDivisor(const T a, const T b) noexcept
    {
        if(a<=0)
        {
            if(b<=0)
            {
                return 1;
            }
            else
            {
                return b;
            }
        }
        else
        {
            assert(a>0);
            if(b<=0)
            {
                return a;
            }
            else
            {
                assert(b>0);
                return Core::GreatestCommonDivisor(a,b);
            }
        }

    }

}

#endif
