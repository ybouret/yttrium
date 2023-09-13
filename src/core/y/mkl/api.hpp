//! \file

#ifndef Y_MKL_API_Included
#define Y_MKL_API_Included 1

#include "y/mkl/complex.hpp"
#include "y/mkl/xreal.hpp"
#include "y/apex/rational.hpp"

namespace Yttrium
{

    namespace MKL
    {
        //______________________________________________________________________
        //
        //
        //! Default Scalar Type
        //
        //______________________________________________________________________
        template <typename T>
        struct ScalarFor
        {
            typedef T Type; //!< self type
        };

        //______________________________________________________________________
        //
        //
        //! Scalar type for Complexes
        //
        //______________________________________________________________________
        template <typename T>
        struct ScalarFor< Complex<T> >
        {
            typedef T Type; //!< base type
        };

    }

}

namespace Yttrium
{

    namespace MKL
    {

        //______________________________________________________________________
        //
        //
        //! Fabs declaration
        //
        //______________________________________________________________________
        template <typename T> struct Fabs;

        //______________________________________________________________________
        //
        //
        //! Fabs<float>
        //
        //______________________________________________________________________
        template <>
        struct Fabs<float>
        {
            //! fabsf
            static inline float Of(const float x) noexcept { return fabsf(x); }
        };

        //______________________________________________________________________
        //
        //
        //! Fabs<double>
        //
        //______________________________________________________________________
        template <>
        struct Fabs<double>
        {
            //! fabs
            static inline double Of(const double x) noexcept { return fabs(x); }
        };

        //______________________________________________________________________
        //
        //
        //! Fabs<long double>
        //
        //______________________________________________________________________
        template <>
        struct Fabs<long double>
        {
            //! fabsl
            static inline long double Of(const long double x) noexcept { return fabsl(x); }
        };


        //______________________________________________________________________
        //
        //
        //! Fabs for XReal<float|double|long double>
        //
        //______________________________________________________________________
        template <typename T>
        struct Fabs< XReal<T> >
        {
            //! use built-in functions
            static inline XReal<T> Of(const XReal<T> x) noexcept { return x.abs(); }
        };


        //______________________________________________________________________
        //
        //
        //! Fabs for apn
        //
        //______________________________________________________________________
        template <>
        struct Fabs<apn>
        {
            //! use buil-in function
            static inline apn Of(const apn &n) { return n; }
        };

        //______________________________________________________________________
        //
        //
        //! Fabs for apz
        //
        //______________________________________________________________________
        template <>
        struct Fabs<apz>
        {
            //! use buil-in function
            static inline apz Of(const apz &z) { return z.abs(); }
        };

        //______________________________________________________________________
        //
        //
        //! Fabs for apq
        //
        //______________________________________________________________________
        template <>
        struct Fabs<apq>
        {
            //! use built-in function
            static inline apq Of(const apq &q) { return q.abs(); }
        };


        

        //______________________________________________________________________
        //
        //
        //! Sqrt for <float|double|long double>
        //
        //______________________________________________________________________
        template <typename T> struct Sqrt
        {
            //! use std function
            static inline T Of(const T &x) noexcept { return std::sqrt(x); }
        };

        //______________________________________________________________________
        //
        //
        //! Sqrt for XReal<float|double|long double>
        //
        //______________________________________________________________________
        template <typename T> struct Sqrt< XReal<T> >
        {
            //! use buil-in functions
            static inline XReal<T> Of(const XReal<T> &x) noexcept { return x.sqrt(); }
        };


        //______________________________________________________________________
        //
        //
        //! hypotenuse for [Xreal]<float|double|long double>
        //
        //______________________________________________________________________
        template <typename T>
        inline T Hypotenuse(const T &a, const T &b) noexcept
        {
            const T one(1);
            const T absa=Fabs<T>::Of(a);
            const T absb=Fabs<T>::Of(b);
            if (absa > absb)
            {
                const T rho = absb/absa;
                const T arg = one + rho*rho;
                return absa*Sqrt<T>::Of(arg);
            }
            else
            {
                const T zero(0);
                if(absb<=zero)
                {
                    return T(0);
                }
                else
                {
                    const T rho = absa/absb;
                    const T arg = one + rho*rho;
                    return absb*Sqrt<T>::Of(arg);
                }
            }
        }

        //______________________________________________________________________
        //
        //
        //! Fabs for any complexes
        //
        //______________________________________________________________________
        template <typename T> struct Fabs< Complex<T> >
        {
            //! use hypotenuse
            static inline T Of(const Complex<T> z) noexcept  { return Hypotenuse(z.re,z.im); }
        };

        //______________________________________________________________________
        //
        //
        //! Default Module^2
        //
        //______________________________________________________________________
        template <typename T> struct Mod2
        {
            //! default squared
            static inline T Of(const T &x) { return x*x; }
        };


        //______________________________________________________________________
        //
        //
        //! Module^2 for complexes
        //
        //______________________________________________________________________
        template <typename T> struct Mod2< Complex<T> >
        {
            //! use built-in function
            static inline T Of(const Complex<T> &z) noexcept { return z.mod2(); }
        };


    }
}

#endif
