
//! \file

#ifndef Y_MKL_Opt_Minimize_Included
#define Y_MKL_Opt_Minimize_Included 1

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
        //! Minimize
        //
        //
        //______________________________________________________________________
        template <typename T>
        struct Minimize
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
            //
            //!
            //
            //__________________________________________________________________
            static T Locate(Triplet<T> &x, Triplet<T> &f, FunctionType &F);

            //__________________________________________________________________
            //
            //
            //! Step for any Callable function
            //
            //__________________________________________________________________
            template <typename FUNCTION> static inline
            T Locate(FUNCTION &F, Triplet<T> &x, Triplet<T> &f )
            {
                Wrapper1D<T,T,FUNCTION> FW(F);
                return Locate(x,f,FW);
            }


        };
    }

}

#endif
