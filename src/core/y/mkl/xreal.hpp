
//! \file

#ifndef Y_Type_XReal_Included
#define Y_Type_XReal_Included 1

#include "y/config/starting.hpp"
#include <iostream>

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //! Extended Real
    //
    //__________________________________________________________________________
    template <typename T>
    class XReal
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        XReal() noexcept;                          //!< zero
        XReal(const T x);                          //!< decompose
        XReal(const XReal &) noexcept;             //!< copy
        XReal & operator=(const XReal &) noexcept; //!< assign
        ~XReal() noexcept;                         //!< cleanup

        inline friend XReal operator*(const XReal &lhs, const XReal &rhs) noexcept
        { return Mul(lhs,rhs); }

        inline XReal & operator*=(const XReal &rhs) noexcept { return (*this=Mul(*this,rhs)); }

        inline friend std::ostream &operator<<(std::ostream &os, const XReal &xr)
        {
            os << '(' << xr.mantissa;
            switch(xr.exponent)
            {
                case -1: os << "/2"; break;
                case  0: break;
                case  1: os << "*2"; break;
                default:
                    if(xr.exponent>0)
                        os << "*2^" << xr.exponent;
                    else
                        os << "/2^" << xr.exponent;
            }
            os << ')';
            return os;
        }
        //______________________________________________________________________
        //
        //
        // members
        //
        //______________________________________________________________________
        const int exponent; //!< exponent
        const T   mantissa; //!< mantissa
        
    private:
        static XReal Mul(const XReal &, const XReal &) noexcept;
    };

}

#endif

