
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
        // Methods
        //
        //______________________________________________________________________

        //! display
        inline friend std::ostream &operator<<(std::ostream &os, const XReal &xr)
        { xr.display(os); return os; }

        //! retrieveing floaint point
        operator T() const;

        //! absolute value
        XReal abs() const noexcept;

        //______________________________________________________________________
        //
        //
        // Multiplication
        //
        //______________________________________________________________________

        //! binary multiplication
        inline friend XReal operator*(const XReal &lhs, const XReal &rhs) noexcept
        { return Mul(lhs,rhs); }

        //! in-place multiplication
        inline XReal & operator*=(const XReal &rhs) noexcept { return (*this=Mul(*this,rhs)); }

        //______________________________________________________________________
        //
        //
        // Division
        //
        //______________________________________________________________________

        //! binary division
        inline friend XReal operator/(const XReal &lhs, const XReal &rhs) noexcept
        { return Div(lhs,rhs); }

        //! in-place division
        inline XReal & operator/=(const XReal &rhs) noexcept { return (*this=Div(*this,rhs)); }

        //______________________________________________________________________
        //
        //
        // Addition
        //
        //______________________________________________________________________

        //! unary +
        XReal operator+() const noexcept;

        //! binary addition
        inline friend XReal operator+(const XReal &lhs, const XReal &rhs) noexcept
        { return Add(lhs,rhs); }

        //! in-place addition
        inline XReal & operator+=(const XReal &rhs) noexcept { return (*this=Add(*this,rhs)); }

        //______________________________________________________________________
        //
        //
        // Subtraction
        //
        //______________________________________________________________________

        //! unary -
        XReal operator-() const noexcept;

        //! binary subtraction
        inline friend XReal operator-(const XReal &lhs, const XReal &rhs) noexcept
        { return Sub(lhs,rhs); }

        //! in-place subtraction
        inline XReal & operator-=(const XReal &rhs) noexcept { return (*this=Sub(*this,rhs)); }
        //______________________________________________________________________
        //
        //
        // members
        //
        //______________________________________________________________________
        const int exponent; //!< exponent
        const T   mantissa; //!< mantissa
        
    private:
        explicit XReal(const int, const T) noexcept;
        static XReal Mul(const XReal &, const XReal &) noexcept;
        static XReal Div(const XReal &, const XReal &);
        static XReal Add(const XReal &, const XReal &);
        static XReal Sub(const XReal &, const XReal &);
        void   display(std::ostream &) const;
    };

}

#endif

