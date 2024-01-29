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
            //__________________________________________________________________
            //
            //
            //! indicate which variables are used
            //
            //__________________________________________________________________
            typedef Readable<bool> Booleans;

            //__________________________________________________________________
            //
            //
            //
            //! Types to perform Fits
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA,typename ORDINATE>
            struct TypeFor
            {
                typedef Readable<ABSCISSA>                    Parameters;         //!< alias
                typedef ABSCISSA                              T1_;                //!< alias
                typedef const Parameters  &                   T2_;                //!< alias
                typedef const Variables   &                   T3_;                //!< alias
                typedef const Booleans    &                   T4_;                //!< alias
                typedef TL3(T1_,T2_,T3_)                      OutOfOrderArgs;     //!< alias
                typedef Functor<ORDINATE,OutOfOrderArgs>      OutOfOrderFunc;     //!< alias
                typedef Writable<ORDINATE>                    GradientType;       //!< alias
                typedef TL5(GradientType&,T1_,T2_,T3_,T4_)    OutOfOrderGradArgs; //!< alias
                typedef Functor<void,OutOfOrderGradArgs>      OutOfOrderGradient; //!< alias

                //! number of coordinates
                static  const unsigned Dimension = sizeof(ORDINATE)/sizeof(ABSCISSA);
            };


        }

    }

}

#endif
