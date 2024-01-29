//! \file

#ifndef Y_Fit_Type_For_Included
#define Y_Fit_Type_For_Included 1

#include "y/mkl/fit/variables.hpp"
#include "y/container/readable.hpp"
#include "y/functor.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {
            typedef Readable<bool> Booleans;

            template <typename ABSCISSA,typename ORDINATE>
            struct TypeFor
            {
                typedef Readable<ABSCISSA>                    Parameters;
                typedef ABSCISSA                              T1_;
                typedef const Parameters  &                   T2_;
                typedef const Variables   &                   T3_;
                typedef const Booleans    &                   T4_;
                typedef TL3(T1_,T2_,T3_)                      OutOfOrderArgs;
                typedef Functor<ORDINATE,OutOfOrderArgs>      OutOfOrderFunc;
                typedef Writable<ORDINATE>                    GradientType;
                typedef TL5(GradientType&,T1_,T2_,T3_,T4_)    OutOfOrderGradArgs;
                typedef Functor<void,OutOfOrderGradArgs>      OutOfOrderGradient;



                static  const unsigned Dimension = sizeof(ORDINATE)/sizeof(ABSCISSA);
            };


        }

    }

}

#endif
