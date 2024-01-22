
//! \file

#ifndef Y_MKL_Opt_Minimize_Included
#define Y_MKL_Opt_Minimize_Included 1

#include "y/mkl/triplet.hpp"
#include "y/mkl/function-wrapper-1d.hpp"

namespace Yttrium
{
    namespace MKL
    {

        struct Minimizing
        {
            static bool Verbose;
        };

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
            static bool Found(Triplet<T> &x, Triplet<T> &f, FunctionType &F);


            template <typename FUNCTION>  inline
            static bool Found(FUNCTION &F, Triplet<T> &x, Triplet<T> &f )
            {
                Wrapper1D<T,T,FUNCTION> FW(F);
                return Found(x,f,FW);
            }

            static T Find(Triplet<T> &x, Triplet<T> &f, FunctionType &F);
            template <typename FUNCTION>  inline

            static T Find(FUNCTION &F, Triplet<T> &x, Triplet<T> &f )
            {
                Wrapper1D<T,T,FUNCTION> FW(F);
                return Find(x,f,FW);
            }


        };
    }

}

#endif
