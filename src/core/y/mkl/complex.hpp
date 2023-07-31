
//! \file

#ifndef Y_Type_Complex_Included
#define Y_Type_Complex_Included 1

#include "y/type/signs.hpp"
#include <iostream>
#include <cmath>

namespace Yttrium
{
    namespace Core
    {
        //! common data for complexes
        struct Complex
        {
            static const char LPAREN = '('; //!< alias
            static const char RPAREN = ')'; //!< alias
            static const char PLUS   = '+'; //!< alias
            static const char IM[4];        //!< "im"
        };
    }
    //__________________________________________________________________________
    //
    //
    //! Complex number
    //
    //__________________________________________________________________________
    template <typename T>
    class Complex
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef T             Type; //!< alias
        typedef Core::Complex Self; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline Complex()                     noexcept : re(0), im(0)       {} //!< zero
        inline Complex(const T x)            noexcept : re(x), im(0)       {} //!< from scalar
        inline Complex(const T x, const T y) noexcept : re(x), im(y)       {} //!< from scalars
        inline Complex(const Complex &z)     noexcept : re(z.re), im(z.im) {} //!< from complex
        inline ~Complex()                    noexcept                      {} //!< cleanup

        inline Complex &operator=(const Complex z) noexcept { re=z.re; im=z.im; return *this; } //!< assign complex
        inline Complex &operator=(const T       x) noexcept { re=x;    im=0;    return *this; } //!< assign scalar

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! no-throw exchange
        inline void xch( Complex &z ) noexcept { Swap(re,z.re); Swap(im,z.im); }

        //! display
        friend std::ostream & operator<<(std::ostream &os, const Complex &z)
        {
            switch( Sign::Of(z.im ))
            {
                case __Zero__: os << z.re; break;
                case Positive: os << Self::LPAREN << z.re << Self::PLUS << z.im << Self::IM << Self::RPAREN; break;
                case Negative: os << Self::LPAREN << z.re <<               z.im << Self::IM << Self::RPAREN; break;
            }
            return os;
        }

        inline Complex conj() const noexcept { return Complex(re,-im); } //!< conjugate
        inline T       mod2() const noexcept { return im*im + re*re;   } //!< |z|^2


        //______________________________________________________________________
        //
        //
        // Add
        //
        //______________________________________________________________________

        //! unary plus
        inline Complex operator+() const noexcept { return Complex(re,im); }

        //! z+z
        friend inline Complex operator+(const Complex &lhs, const Complex &rhs) noexcept
        { return Complex(lhs.re+rhs.re,lhs.im+rhs.im); }

        //! z+x
        friend inline Complex operator+(const Complex &lhs, const T rhs) noexcept
        { return Complex(lhs.re+rhs,lhs.im); }

        //! x+z
        friend inline Complex operator+(const T  lhs, const Complex &rhs) noexcept
        { return Complex(lhs+rhs.re,rhs.im); }

        //! += z
        inline Complex & operator+=(const Complex &z) noexcept
        { re += z.re; im += z.im; return *this; }

        //! += x
        inline Complex & operator+=(const T x) noexcept
        { re += x; return *this; }



        //______________________________________________________________________
        //
        //
        // Sub
        //
        //______________________________________________________________________

        //! unary plus
        inline Complex operator-() const noexcept { return Complex(-re,-im); }

        //! z-z
        friend inline Complex operator-(const Complex &lhs, const Complex &rhs) noexcept
        { return Complex(lhs.re-rhs.re,lhs.im-rhs.im); }

        //! z-x
        friend inline Complex operator-(const Complex &lhs, const T rhs) noexcept
        { return Complex(lhs.re-rhs,lhs.im); }

        //! x-z
        friend inline Complex operator-(const T  lhs, const Complex &rhs) noexcept
        { return Complex(lhs-rhs.re,-rhs.im); }

        //! -= z
        inline Complex & operator-=(const Complex &z) noexcept
        { re -= z.re; im -= z.im; return *this; }

        //! -= x
        inline Complex & operator-=(const T x) noexcept
        { re -= x; return *this; }

        //______________________________________________________________________
        //
        //
        // Mul
        //
        //______________________________________________________________________

        //! z*x
        inline friend Complex operator*(const Complex &z, const T x) noexcept
        { return Complex(z.re*x,z.im*x); }

        //! x*z*
        inline friend Complex operator*(const T x, const Complex &z) noexcept
        { return Complex(z.re*x,z.im*x); }

        //! standard multiplication
        inline static Complex MultiAlgo(const Complex &X, const Complex &Y) noexcept
        { return Complex(X.re * Y.re - X.im * Y.im, X.re * Y.im + X.im * Y.re); }

        //! Gauss multiplication
        inline static Complex GaussAlgo(const Complex &X, const Complex &Y) noexcept
        {
            const T S1 = X.re * Y.re;
            const T S2 = X.im * Y.im;
            const T S3 = (X.re+X.im) * (Y.re+Y.im);
            return Complex(S1-S2,S3-S1-S2);
        }

        //! X*Y*
        inline friend Complex operator*(const Complex &X, const Complex &Y) noexcept
        { return Complex(X.re * Y.re - X.im * Y.im, X.re * Y.im + X.im * Y.re); }

        //! *= x
        inline Complex & operator*=(const T x)        { re *= x; im *= x; return *this; }

        //! *= z
        inline Complex & operator*=(const Complex &z)
        { const Complex p = (*this) * z; re=p.re; im=p.im; return *this; }

        //______________________________________________________________________
        //
        //
        // Division
        //
        //______________________________________________________________________

        //! z/x
        inline friend Complex operator/(const Complex &z, const T x)
        {
            // TODO: check
            return Complex(z.re/x,z.im/x);
        }

        //! x/z
        inline friend Complex operator/(const T x, const Complex &z)
        {
            return (x*z.conj())/z.mod2();
        }

        //! X/Y
        inline friend Complex operator/(const Complex &X, const Complex &Y)
        {
            return (X * Y.conj())/Y.mod2();
        }

        //! /= x
        inline Complex & operator/=(const T x)        { re/=x; im/=x; return *this; }

        //! /= z
        inline Complex & operator/=(const Complex &z)
        {
            const Complex d = (*this) / z; re = d.re; im = d.im; return *this;
        }


        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        

        T re; //!< real part
        T im; //!< imaginary part
    };
}

#endif
