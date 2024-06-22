//! \file

#ifndef Y_MKL_API_Included
#define Y_MKL_API_Included 1

#include "y/mkl/complex.hpp"
#include "y/mkl/xreal.hpp"
#include "y/kemp/rational.hpp"
#include "y/sort/nw.hpp"

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
        //! Fabs<int>
        //
        //______________________________________________________________________
        template <>
        struct Fabs<int>
        {
            //! abs
            static inline int Of(const int x) noexcept { return abs(x); }
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
        //! Sqrt declaration
        //
        //______________________________________________________________________
        template <typename T> struct Sqrt;

        //______________________________________________________________________
        //
        //
        //! Sqrt for float
        //
        //______________________________________________________________________
        template <> struct Sqrt<float>
        {
            //! use std function
            static inline float Of(const float &x) noexcept { return sqrtf(x); }
        };

        //______________________________________________________________________
        //
        //
        //! Sqrt for double
        //
        //______________________________________________________________________
        template <> struct Sqrt<double>
        {
            //! use std function
            static inline double Of(const double &x) noexcept { return sqrt(x); }
        };

        //______________________________________________________________________
        //
        //
        //! Sqrt for long double
        //
        //______________________________________________________________________
        template <> struct Sqrt<long double>
        {
            //! use std function
            static inline long double Of(const long double &x) noexcept { return sqrtl(x); }
        };



        //______________________________________________________________________
        //
        //
        //! Sqrt for XReal<float|double|long double>
        //
        //______________________________________________________________________
        template <typename T> struct Sqrt< XReal<T> >
        {
            //! use built-in functions
            static inline XReal<T> Of(const XReal<T> &x) noexcept { return x.sqrt(); }
        };


        //______________________________________________________________________
        //
        //
        //! Sqrt for rational
        //
        //______________________________________________________________________
        template<> struct Sqrt<apq>
        {
            //! use apq methods
            static inline apq Of(const apq &x) { return x.sqrt(); }
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
            if(absa > absb)
            {
                // |a| > |b|
                const T rho = absb/absa;
                const T arg = one + rho*rho;
                return absa*Sqrt<T>::Of(arg);
            }
            else
            {
                // |a| <= |b|
                const T zero(0);
                if(absb<=zero)
                {
                    return zero;
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
        //! hypotenuse for [Xreal]<float|double|long double>
        //
        //______________________________________________________________________
        template <typename T>
        inline T Hypotenuse(const T &a, const T &b, const T &c) noexcept
        {
            static const T zero(0);
            static const T one(1);

            T data[4] = { zero, Fabs<T>::Of(a), Fabs<T>::Of(b), Fabs<T>::Of(c) };
            NetworkSort::Algo<3>::Decreasing(data);
            const T dmax = data[1];
            if(dmax<=zero)
                return zero;
            else
            {
                const T rho2 = data[2]/dmax;
                const T rho3 = data[3]/dmax;
                const T args = one + (rho2*rho2 + rho3*rho3);
                return dmax * Sqrt<T>::Of(args);
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

        //______________________________________________________________________
        //
        //
        //! return the signed value of a w.r.t the sign of b
        //
        //______________________________________________________________________
        template <typename T, typename U>
        inline T Sgn(T a, U b) throw()
        {
            const U zero(0);
            return (b >= zero) ? Fabs<T>::Of(a) : -Fabs<T>::Of(a);
        }
    }

    namespace MKL
    {
        template <typename> struct Floor;

        //______________________________________________________________________
        //
        //
        //! Floort for float
        //
        //______________________________________________________________________     
        template <> struct Floor<float>
        {
            //! use std function
            static inline float Of(const float &x) { return floorf(x); }
        };

        //______________________________________________________________________
        //
        //
        //! Floort for double
        //
        //______________________________________________________________________
        template <> struct Floor<double>
        {
            //! use std function

            static inline double Of(const double &x) { return floor(x); }
        };

        //______________________________________________________________________
        //
        //
        //! Floort for long double
        //
        //______________________________________________________________________    
        template <> struct Floor<long double>
        {
            //! use std function
            static inline long double Of(const long double &x) { return floorl(x); }
        };
    }
}

#endif
