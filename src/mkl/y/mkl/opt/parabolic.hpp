
//! \file

#ifndef Y_MKL_Opt_Parabolic_Included
#define Y_MKL_Opt_Parabolic_Included 1

#include "y/mkl/triplet.hpp"
#include "y/mkl/function-wrapper.hpp"

namespace Yttrium
{
    namespace MKL
    {

        template <typename T>
        struct Parabolic
        {
            typedef Function<T,T> FunctionType;

            static void Step(Triplet<T> &x, Triplet<T> &f, FunctionType &F);

            template <typename FUNCTION> static inline
            void Step(FUNCTION &F, Triplet<T> &x, Triplet<T> &f )
            {
                Wrapper<T,T,FUNCTION> FW(F);
                Step(x,f,FW);
            }


        };

    }
}

#endif
