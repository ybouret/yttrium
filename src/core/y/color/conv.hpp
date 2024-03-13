
//! \file

#ifndef Y_Color_Conv_Included
#define Y_Color_Conv_Included 1

#include "y/mkl/api.hpp"

namespace Yttrium
{
    namespace Color
    {
        //! T=[float|double|long double]
        template <typename T>
        struct Conv
        {
            static const T Unit[256];
            static inline uint8_t ToByte(const T x) noexcept
            {
                static const T half(0.5);
                static const T full(255);
                const T arg = full * x + half;
                return floor(arg);
            }
        };
        
#if !defined(_MSC_VER)
        template <> const float       Conv<float>      ::Unit[];
        template <> const double      Conv<double>     ::Unit[];
        template <> const long double Conv<long double>::Unit[];
#endif
    }

}

#endif
