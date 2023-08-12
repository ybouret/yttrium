
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

        //______________________________________________________________________
        //
        //
        // Multiplication
        //
        //______________________________________________________________________

        inline friend XReal operator*(const XReal &lhs, const XReal &rhs) noexcept
        { return Mul(lhs,rhs); }

        inline XReal & operator*=(const XReal &rhs) noexcept { return (*this=Mul(*this,rhs)); }

        // display
        inline friend std::ostream &operator<<(std::ostream &os, const XReal &xr)
        {
            xr.display(os);
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
        void   display(std::ostream &) const;
    };

}

#endif

