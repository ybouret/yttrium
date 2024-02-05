//! \file

#ifndef Y_MKL_V4D_Included
#define Y_MKL_V4D_Included 1

#include "y/mkl/v3d.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! 4D Vertex
    //
    //
    //__________________________________________________________________________
    template <typename T>
    class V4D
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
        inline  V4D()                                           noexcept : x(0), y(0), z(0), w(0) {}                   //!< setup 0
        inline  V4D(const T X, const T Y, const T Z, const T W) noexcept : x(X), y(Y), z(Z), w(W) {}                   //!< setup
        inline ~V4D()                                           noexcept {}                                            //!< cleanup
        inline  V4D(const V4D &v)                               noexcept : x(v.x), y(v.y), z(v.z), w(v.w) {}           //!< copy
        inline  V4D & operator=(const V4D &v)                   noexcept { x=v.x; y=v.y; z=v.z; w=v.w; return *this; } //!< assign

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! pseudo writable
        inline size_t size() const noexcept { return 4; }

        //______________________________________________________________________
        //
        // access
        //______________________________________________________________________
        inline T &       operator[](const size_t i)       noexcept { assert(i==1||i==2||i==3||i==4); return *((&x)+i-1); } //!< access
        inline const T & operator[](const size_t i) const noexcept { assert(i==1||i==2||i==3||i==4); return *((&x)+i-1); } //!< access

        //______________________________________________________________________
        //
        // addition
        //______________________________________________________________________
        inline V4D        operator+() const noexcept 
        { return V4D(x,y,z,w); }                                         //!< unary +

        inline friend V4D operator+(const V4D  &lhs, const V4D &rhs) noexcept
        { return V4D(lhs.x+rhs.x,lhs.y+rhs.y,lhs.z+rhs.z,lhs.w+rhs.w); } //!< binary +
        
        inline V4D &      operator+=(const V4D &rhs) noexcept
        { x+=rhs.x; y+=rhs.y; z+=rhs.z; w+=rhs.w; return *this; }        //!< in place +

        //______________________________________________________________________
        //
        // subtraction
        //______________________________________________________________________
        inline V4D        operator-()                          const noexcept
        { return V4D(-x,-y,-z,-w); }                                     //!< unary minus

        inline friend V4D operator-(const V4D  &lhs, const V4D &rhs) noexcept
        { return V4D(lhs.x-rhs.x,lhs.y-rhs.y,lhs.z-rhs.z,lhs.w-rhs.w); } //!< binary -

        inline V4D & operator-=(const V4D &rhs) noexcept
        { x-=rhs.x; y-=rhs.y; z-=rhs.z; w -= rhs.w; return *this; }      //!< in place -

        //______________________________________________________________________
        //
        // multiplication
        //______________________________________________________________________
        inline friend V4D operator*(const T u, const V4D &v)    noexcept { return V4D(v.x*u,v.y*u,v.z*u,v.w*u);          } //!< left multiplication
        inline V4D &      operator*=(const T u)                 noexcept { x*=u; y*=u; z*=u; w*=u; return *this;         } //!< in place multiplication
        inline friend T   operator*(const V4D &a, const V4D &b) noexcept { return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w; } //!< dot product

        //______________________________________________________________________
        //
        // division
        //______________________________________________________________________
        inline friend V4D operator/( const V4D &v, const T u)   noexcept { return V4D(v.x/u,v.y/u,v.z/u,v.w/u);  } //!< right division
        inline V4D &      operator/=(const T u)                 noexcept { x/=u; y/=u; z/=u; w/=u; return *this; } //!< in place division


        //______________________________________________________________________
        //
        //! display
        //______________________________________________________________________
        inline friend std::ostream & operator<<(std::ostream &os, const V4D &v)
        { return os << '[' << v.x << ';' << v.y << ';' << v.z << ';' << v.w << ']'; }

        //______________________________________________________________________
        //
        //! equality
        //______________________________________________________________________
        inline friend bool operator==(const V4D &lhs, const V4D &rhs) noexcept
        {
            return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;;
        }

        //______________________________________________________________________
        //
        //! difference
        //______________________________________________________________________
        inline friend bool operator!=(const V4D &lhs, const V4D &rhs) noexcept
        {
            return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z || lhs.w != rhs.w;
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
        T w; //!< fourth
    };


}

#endif

