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

            //__________________________________________________________________
            //
            //
            //! performing wary sum3
            //
            //__________________________________________________________________
            template <typename T>
            struct Sum3Proxy<T,true>
            {
                Y_ARGS_DECL(T,Type);                                      //!< aliases
                typedef typename ScalarFor<MutableType>::Type ScalarType; //!< alias

                //! make sum3 by probing largest elememnt
                static inline Type Of(ConstType &a, ConstType &b, ConstType &c)
                {
                    Big        big  = BigA;
                    {
                        ScalarType amx  = Fabs<T>::Of(a);
                        {
                            const ScalarType tmp = Fabs<T>::Of(b);
                            if(tmp>amx) { amx=tmp; big=BigB; }
                        }
                        {
                            const ScalarType tmp = Fabs<T>::Of(c);
                            if(tmp>amx) {  big=BigC; }
                        }
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

                //! sum3 of absolute values by probing largest
                static inline ScalarType OfAbs(ConstType &a, ConstType &b, ConstType &c)
                {
                    ScalarType       vmax = Fabs<T>::Of(a);
                    const ScalarType absa = vmax;           Big             big = BigA;
                    const ScalarType absb = Fabs<T>::Of(b); if(absb>vmax) { big = BigB; vmax=absb; }
                    const ScalarType absc = Fabs<T>::Of(c); if(absc>vmax) { big = BigC; vmax=absc; }
                    switch(big)
                    {
                        case BigA: return (absb+absc) + absa;
                        case BigB: return (absc+absa) + absb;
                        case BigC: return (absa+absb) + absc;
                    }
                    // never get here
                    return absa+absb+absc;
                }


            private:
                enum Big
                {
                    BigA,
                    BigB,
                    BigC
                };
            };

            //__________________________________________________________________
            //
            //
            //! performing direct sum3
            //
            //__________________________________________________________________
            template <typename T>
            struct Sum3Proxy<T,false>
            {
                Y_ARGS_DECL(T,Type); //!< aliases
                typedef typename ScalarFor<MutableType>::Type ScalarType; //!< alias

                //! direct sum3
                static inline Type Of(ConstType &a, ConstType &b, ConstType &c)
                {
                    return a+b+c;
                }

                //! direct sum3 of absolute values
                static inline ScalarType OfAbs(ConstType &a, ConstType &b, ConstType &c)
                {
                    return Fabs<T>::Of(a) + Fabs<T>::Of(b) + Fabs<T>::Of(c);
                }

            };

            //__________________________________________________________________
            //
            //
            //! selecting sum3 algorithm
            //
            //__________________________________________________________________
            template <typename T>
            struct Sum3
            {
                Y_ARGS_DECL(T,Type);                                      //!< aliases
                typedef typename ScalarFor<MutableType>::Type ScalarType; //!< alias


                //! algorithm selection
                static inline T Of(ParamType a, ParamType b, ParamType c)
                {
                    return Sum3Proxy<T, Wary<T>::Flag >::Of(a,b,c);
                }

                //! algorithm selection
                static inline ScalarType OfAbs(ParamType a, ParamType b, ParamType c)
                {
                    return Sum3Proxy<Type, Wary<Type>::Flag >::OfAbs(a,b,c);
                }

            };

        }
    }

}




#endif

