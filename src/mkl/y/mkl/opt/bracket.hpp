
//! \file

#ifndef Y_MKL_Opt_Bracket_Included
#define Y_MKL_Opt_Bracket_Included 1

#include "y/mkl/triplet.hpp"
#include "y/mkl/function-wrapper.hpp"

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

            //! bracket withing f.a = F(x.a), f.c = F(x.c)
            /**
             - return true for regular local minimum
             - return false for global minimum, on lowest side
             */
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
                Wrapper<T,T,FUNCTION> FW(F);
                return Inside(x,f,FW);
            }
        };
    }

}

#endif

