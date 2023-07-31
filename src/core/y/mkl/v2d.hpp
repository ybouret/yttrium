
//! \file

#ifndef Y_MKL_V2D_Included
#define Y_MKL_V2D_Included 1

#include "y/config/starting.hpp"
#include <iostream>
#include <cmath>

namespace Yttrium
{

    template <typename T>
    class V2D
    {
    public:

        // C++
        inline  V2D(const T X, const T Y)     noexcept : x(X), y(Y) {}
        inline ~V2D()                         noexcept {}
        inline  V2D(const V2D &v)             noexcept : x(v.x), y(v.y) {}
        inline  V2D & operator=(const V2D &v) noexcept { x=v.x; y=v.y; return *this; }

        // access
        inline T &       operator[](const size_t i)       noexcept { assert(i==1||i==2); return (&x)+i-1; }
        inline const T & operator[](const size_t i) const noexcept { assert(i==1||i==2); return (&x)+i-1; }

        // addition
        inline V2D        operator+()                         const noexcept { return V2D(x,y); }
        inline friend V2D operator+(const V2D &lhs, const V2D &rhs) noexcept { return V2D(lhs.x+rhs.x,lhs.y+rhs.y); }
        inline V2D & operator+=(const V2D &rhs)                     noexcept { x+=rhs.x; y+=rhs.y; return *this; }

        // subtraction
        inline V2D        operator-()                         const noexcept { return V2D(-x,-y); }
        inline friend V2D operator-(const V2D &lhs, const V2D &rhs) noexcept { return V2D(lhs.x-rhs.x,lhs.y-rhs.y); }
        inline V2D & operator-=(const V2D &rhs)                     noexcept { x-=rhs.x; y-=rhs.y; return *this; }

        inline friend std::ostream & operator<<(std::ostream &os, const V2D &v)
        { return os << '[' << v.x << ';' << v.y << ']';         }

        // members
        T x;
        T y;
    };

}

#endif

