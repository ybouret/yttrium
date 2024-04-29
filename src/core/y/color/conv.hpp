
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
            static const T Unit[256]; //!< [0..255]/255

            //! [0..1] to [0..255]
            static inline uint8_t ToByte(const T x) noexcept
            {
                static const T half(0.5);
                static const T full(255);
                const T arg = full * x + half;
                return floor(arg);
            }

            static const  T Gray[255*3+1]; //!< [0..255*3]/(255*3);
        };

        //! 8-bits precomputed table
        struct Conv8
        {
            static const uint8_t Gray[255*3+1]; //!< ([0..255]*765)/765
        };



#if !defined(_MSC_VER)
        template <> const float       Conv<float>      ::Unit[]; //!< decl
        template <> const double      Conv<double>     ::Unit[]; //!< decl
        template <> const long double Conv<long double>::Unit[]; //!< decl

        template <> const float       Conv<float>      ::Gray[]; //!< decl
        template <> const double      Conv<double>     ::Gray[]; //!< decl
        template <> const long double Conv<long double>::Gray[]; //!< decl
#endif
    }

}

#endif
