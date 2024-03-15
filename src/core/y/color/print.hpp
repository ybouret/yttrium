//! \file

#ifndef Y_Color_Print_Included
#define Y_Color_Print_Included 1

#include "y/config/starting.hpp"
#include <iostream>

namespace Yttrium
{
    namespace Color
    {
        //! generic print function for colours
        template <typename T>
        struct Print
        {
            //! call
            static inline void Out(std::ostream &os, const T x)
            {
                os << x;
            }
        };

        //! print uint8_t as unsigned
        template <>
        struct Print<uint8_t>
        {
            //! call
            static void Out(std::ostream &os, const uint8_t x);
        };

        //! print int8_t as integer
        template <>
        struct Print<int8_t>
        {
            //! call
            static void Out(std::ostream &os, const int8_t x);
        };


    }
}

#endif

