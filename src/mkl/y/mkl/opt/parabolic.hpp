//! \file

#ifndef Y_MKL_Opt_Parabolic_Included
#define Y_MKL_Opt_Parabolic_Included 1

#include "y/mkl/triplet.hpp"
#include "y/mkl/function-wrapper-1d.hpp"

namespace Yttrium
{
    namespace MKL
    {

        

        //______________________________________________________________________
        //
        //
        //
        //! Improve local minimum
        //
        //
        //______________________________________________________________________
        template <typename T>
        struct Parabolic
        {
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Function<T,T> FunctionType; //!< alias

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________


            //__________________________________________________________________
            //
            //! parabolic step
            /**
             - start from increasing x and correspond local minimum F
             - update by parabolic estimation and 1 function evaluation
             */
            //__________________________________________________________________
            static bool Step(Triplet<T>   &x,
                             Triplet<T>   &f,
                             FunctionType &F);

            //__________________________________________________________________
            //
            //
            //! Step for any Callable function
            //
            //__________________________________________________________________
            template <typename FUNCTION> static inline
            bool Step(FUNCTION &F, Triplet<T> &x, Triplet<T> &f)
            {
                Wrapper1D<T,T,FUNCTION> FW(F);
                return Step(x,f,FW);
            }


        };

    }
}

#endif
