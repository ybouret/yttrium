//! \file

#ifndef Y_MKL_API_Included
#define Y_MKL_API_Included 1

#include "y/mkl/complex.hpp"

namespace Yttrium
{

    namespace MKL
    {

        template <typename T>
        struct ScalarFor
        {
            typedef T Type;
        };

        template <typename T>
        struct ScalarFor< Complex<T> >
        {
            typedef T Type;
        };


        struct API
        {

#if 0
            template <typename T> static T Fabs(const T&);
            template <typename T> static T Sqrt(const T&);

            template <typename T> static
            T Hypotenuse(const T &a, const T &b)
            {
                const T one(1);
                const T absa=Fabs(a);
                const T absb=Fabs(b);
                if (absa > absb)
                {
                    const T rho = absb/absa;
                    const T arg = one + rho*rho;
                    return absa*Sqrt(arg);
                }
                else
                {
                    if(absb<=0)
                    {
                        return T(0);
                    }
                    else
                    {
                        const T rho = absa/absb;
                        const T arg = one + rho*rho;
                        return absb*Sqrt(arg);
                    }
                }
            }

            template <typename T> struct Mod2
            {
                static inline T Of(const T &x) { return x*x; }
            };

            template <typename T> struct Mod2< Complex<T> >
            {
                static inline T Of(const Complex<T> &x) { return x.re*x.re + x.im*x.im; }
            };
#endif



            



        };

    }
}

#endif
