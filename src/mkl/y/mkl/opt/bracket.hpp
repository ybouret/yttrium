
//! \file

#ifndef Y_MKL_Opt_Bracket_Included
#define Y_MKL_Opt_Bracket_Included 1

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
        //! Bracketing minimum
        //
        //
        //______________________________________________________________________
        template <typename T>
        struct Bracket
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
            //! bracket within f.a = F(x.a), f.c = F(x.c)
            /**
             - return true for regular local minimum, ordered x
             - return false for global minimum, on lowest side
             */
            //__________________________________________________________________
            static bool Inside(Triplet<T>   &x,
                               Triplet<T>   &f,
                               FunctionType &F);
            
            //__________________________________________________________________
            //
            //
            //! Step for any callable function
            //
            //__________________________________________________________________
            template <typename FUNCTION> static inline
            bool Inside(FUNCTION &F, Triplet<T> &x, Triplet<T> &f )
            {
                Wrapper1D<T,T,FUNCTION> FW(F);
                return Inside(x,f,FW);
            }

            //__________________________________________________________________
            //
            //
            //! bracket from f.a=F(x.a) and f.b=F(x.c)
            //
            //__________________________________________________________________
            static void Expand(Triplet<T>   &x,
                               Triplet<T>   &f,
                               FunctionType &F);
            
        };
    }

}

#endif

