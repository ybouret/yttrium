//! \file

#ifndef Y_Color_Print_Included
#define Y_Color_Print_Included 1

#include "y/config/starting.hpp"
#include <iostream>

namespace Yttrium
{
    namespace Color
    {
        template <typename T>
        struct Print
        {
            static inline void Out(std::ostream &os, const T x)
            {
                os << x;
            }
        };

        template <>
        struct Print<uint8_t>
        {
            static void Out(std::ostream &os, const uint8_t x);
        };

        template <>
        struct Print<int8_t>
        {
            static void Out(std::ostream &os, const int8_t x);
        };


    }
}

#endif

