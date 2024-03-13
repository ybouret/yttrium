
//! \file

#ifndef Y_Color_YUV_Included
#define Y_Color_YUV_Included 1

#include "y/config/starting.hpp"
#include <iostream>

namespace Yttrium
{
    namespace Color
    {
        template <typename T>
        class YUV
        {
        public:
            inline ~YUV()                                noexcept {}
            inline  YUV()                                noexcept : y(0), u(0), v(0) {}
            inline  YUV(const T Y, const T U, const T V) noexcept : y(Y), u(U), v(V){}
            inline  YUV(const YUV &c)                    noexcept : y(c.y), u(c.u), v(c.y){}
            inline  YUV & operator=(const YUV &c)        noexcept { y = c.y; u=c.u; v=c.v; return *this; }

            inline friend std::ostream & operator<<(std::ostream &os, const YUV &c)
            {
                os << '[' << c.y << ',' << c.y << ',' << c.v << ']';
                return os;
            }


            T y;
            T u;
            T v;
        };
    }

}

#endif

