

//! \file

#ifndef Y_MKL_V3D_Included
#define Y_MKL_V3D_Included 1

#include "y/mkl/v2d.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! 3D Vertex
    //
    //
    //__________________________________________________________________________
    template <typename T>
    class V3D
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definition
        //
        //______________________________________________________________________
        typedef T             Type; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline  V3D()                                noexcept : x(0), y(0), z(0) {}                  //!< setup 0
        inline  V3D(const T X, const T Y, const T Z) noexcept : x(X), y(Y), z(Z) {}                  //!< setup
        inline ~V3D()                                noexcept {}                                     //!< cleanup
        inline  V3D(const V3D &v)                    noexcept : x(v.x), y(v.y), z(v.z) {}            //!< copy
        inline  V3D & operator=(const V3D &v)        noexcept { x=v.x; y=v.y; z=v.z; return *this; } //!< assign

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! pseudo writable
        inline size_t size() const noexcept { return 3; }

        //______________________________________________________________________
        //
        // access
        //______________________________________________________________________
        inline T &       operator[](const size_t i)       noexcept { assert(i==1||i==2||i==3); return *((&x)+i-1); } //!< access
        inline const T & operator[](const size_t i) const noexcept { assert(i==1||i==2||i==3); return *((&x)+i-1); } //!< access

        //______________________________________________________________________
        //
        // addition
        //______________________________________________________________________
        inline V3D        operator+()                          const noexcept { return V3D(x,y,z); }                               //!< unary +
        inline friend V3D operator+(const V3D  &lhs, const V3D &rhs) noexcept { return V3D(lhs.x+rhs.x,lhs.y+rhs.y,lhs.z+rhs.z); } //!< binary +
        inline V3D &      operator+=(const V3D &rhs)                 noexcept { x+=rhs.x; y+=rhs.y; z+=rhs.z; return *this; }      //!< in place +

        //______________________________________________________________________
        //
        // subtraction
        //______________________________________________________________________
        inline V3D        operator-()                          const noexcept { return V3D(-x,-y,-z); }                            //!< unary minus
        inline friend V3D operator-(const V3D  &lhs, const V3D &rhs) noexcept { return V3D(lhs.x-rhs.x,lhs.y-rhs.y,lhs.z-rhs.z); } //!< binary -
        inline V3D &      operator-=(const V3D &rhs)                 noexcept { x-=rhs.x; y-=rhs.y; z-=rhs.z; return *this; }      //!< in place -

        //______________________________________________________________________
        //
        // multiplication
        //______________________________________________________________________
        inline friend V3D operator*(const T u, const V3D &v)    noexcept { return V3D(v.x*u,v.y*u,v.z*u);      } //!< left multiplication
        inline V3D &      operator*=(const T u)                 noexcept { x*=u; y*=u; z*=u; return *this;     } //!< in place multiplication
        inline friend T   operator*(const V3D &a, const V3D &b) noexcept { return a.x*b.x + a.y*b.y + a.z*b.z; } //!< dot product

        //______________________________________________________________________
        //
        // division
        //______________________________________________________________________
        inline friend V3D operator/( const V3D &v, const T u)   noexcept { return V3D(v.x/u,v.y/u,v.z/u);  } //!< right division
        inline V3D &      operator/=(const T u)                 noexcept { x/=u; y/=u; z/=u; return *this; } //!< in place division


        //______________________________________________________________________
        //
        //! display
        //______________________________________________________________________
        inline friend std::ostream & operator<<(std::ostream &os, const V3D &v)
        { return os << '[' << v.x << ';' << v.y << ';' << v.z << ']'; }

        //______________________________________________________________________
        //
        //! equality
        //______________________________________________________________________
        inline friend bool operator==(const V3D &lhs, const V3D &rhs) noexcept
        {
            return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
        }

        //______________________________________________________________________
        //
        //! difference
        //______________________________________________________________________
        inline friend bool operator!=(const V3D &lhs, const V3D &rhs) noexcept
        {
            return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
        }

        //______________________________________________________________________
        //
        //! |v|^2
        //______________________________________________________________________
        inline T norm2() const noexcept
        {
            T data[4] = { 0, x*x, y*y, z*z };
            NetworkSort::Algo<3>::Decreasing(data);
            return data[1] + (data[2]+data[3]);
        }

        //______________________________________________________________________
        //
        //! |v|
        //______________________________________________________________________
        inline T norm() const noexcept
        {
            return MKL::Hypotenuse(x,y,z);
        }

        //______________________________________________________________________
        //
        //! projection
        //______________________________________________________________________
        inline V2D<T> xy() const noexcept
        {
            return V2D<T>(x,y);
        }

        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________

        T x; //!< first
        T y; //!< second
        T z; //!< third
    };


}

#endif

