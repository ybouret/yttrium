//! \file

#ifndef Y_MKL_Antelope_Sum3_Included
#define Y_MKL_Antelope_Sum3_Included 1

#include "y/mkl/antelope/wary.hpp"
#include "y/mkl/api.hpp"
#include "y/type/args.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Antelope
        {
            template <typename T,const bool> struct Sum3Proxy;


            template <typename T>
            struct Sum3Proxy<T,true>
            {

                typedef typename ScalarFor<T>::Type ScalarType;

                Y_ARGS_DECL(T,Type);
                static inline Type Of(ConstType &a, ConstType &b, ConstType &c)
                {
                    std::cerr << "wary sum3" << std::endl;
                    ScalarType amx  = Fabs<T>::Of(a);
                    Big        big  = BigA;
                    {
                        const ScalarType tmp = Fabs<T>::Of(b);
                        if(tmp>amx) { amx=tmp; big=BigB; }
                    }
                    {
                        const ScalarType tmp = Fabs<T>::Of(c);
                        if(tmp>amx) { amx=tmp; big=BigC; }
                    }
                    switch(big)
                    {
                        case BigA: return (b+c) + a;
                        case BigB: return (c+a) + b;
                        case BigC: return (a+b) + c;
                    }
                    // never get here
                    return a+b+c;
                }

            private:
                enum Big
                {
                    BigA,
                    BigB,
                    BigC
                };
            };

            template <typename T>
            struct Sum3Proxy<T,false>
            {
                Y_ARGS_DECL(T,Type);
                static inline Type Of(ConstType &a, ConstType &b, ConstType &c)
                {
                    std::cerr << "direct sum3" << std::endl;
                    return a+b+c;
                }
            };

            template <typename T>
            struct Sum3
            {
                Y_ARGS_DECL(T,Type);

                static inline T Of(ParamType a, ParamType b, ParamType c)
                {
                    return Sum3Proxy<T, Wary<T>::Flag >::Of(a,b,c);
                }
            };

        }
    }

}




#endif

