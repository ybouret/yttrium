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
            template <typename ABSCISSA,typename ORDINATE>
            struct TypeFor
            {
                typedef Readable<ABSCISSA>               Parameters;
                typedef const Parameters  &              T2_;
                typedef const Variables   &              T3_;
                typedef TL3(ABSCISSA,T2_,T3_)            OutOfOrderArgs;
                typedef Functor<ORDINATE,OutOfOrderArgs> OutOfOrderFunc;
                static  const unsigned Dimension = sizeof(ORDINATE)/sizeof(ABSCISSA);
            };
        }

    }

}

#endif
