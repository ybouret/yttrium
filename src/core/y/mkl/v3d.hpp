

//! \file

#ifndef Y_MKL_V3D_Included
#define Y_MKL_V3D_Included 1

#include "y/mkl/v2d.hpp"
#include <iostream>
#include <cmath>

namespace Yttrium
{
    template <typename T>
    class V3D
    {
    public:

        // C++
        inline  V3D(const T X, const T Y, const T Z) noexcept : x(X), y(Y), z(Z) {}
        inline ~V3D()                                noexcept {}
        inline  V3D(const V3D &v)                    noexcept : x(v.x), y(v.y), z(v.z) {}
        inline  V3D & operator=(const V3D &v)        noexcept { x=v.x; y=v.y; z=v.z; return *this; }

        inline T &       operator[](const size_t i)       noexcept { assert(i==1||i==2||i==3); return (&x)+i-1; }
        inline const T & operator[](const size_t i) const noexcept { assert(i==1||i==2||i==3); return (&x)+i-1; }

        // addition
        inline V3D        operator+()                          const noexcept { return V3D(x,y,z); }
        inline friend V3D operator+(const V3D  &lhs, const V3D &rhs) noexcept { return V3D(lhs.x+rhs.x,lhs.y+rhs.y,lhs.z+rhs.z); }
        inline V3D &      operator+=(const V3D &rhs)                 noexcept { x+=rhs.x; y+=rhs.y; z+=rhs.z; return *this; }

        // subtraction
        inline V3D        operator-()                          const noexcept { return V3D(-x,-y,-z); }
        inline friend V3D operator-(const V3D  &lhs, const V3D &rhs) noexcept { return V3D(lhs.x-rhs.x,lhs.y-rhs.y,lhs.z-rhs.z); }
        inline V3D &      operator-=(const V3D &rhs)                 noexcept { x-=rhs.x; y-=rhs.y; z-=rhs.z; return *this; }

        // display
        inline friend std::ostream & operator<<(std::ostream &os, const V3D &v)
        { return os << '[' << v.x << ';' << v.y << ';' << v.z << ']'; }

        T x;
        T y;
        T z;
    };


}

#endif

