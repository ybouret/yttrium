
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
        //! common data
        //
        //
        //______________________________________________________________________
        struct Minimizing
        {
            static bool Verbose; //!< verbosity during minimize
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
           
            //! reduction/interpolation convergence
            static bool Found(Triplet<T> &x, Triplet<T> &f, FunctionType &F);


            //! wrapper
            template <typename FUNCTION>  inline
            static bool Found(FUNCTION &F, Triplet<T> &x, Triplet<T> &f )
            {
                Wrapper1D<T,T,FUNCTION> FW(F);
                return Found(x,f,FW);
            }

            //! starting from local minimum estimation, converge
            static T Find(Triplet<T> &x, Triplet<T> &f, FunctionType &F);
            template <typename FUNCTION>  inline

            //! wrapper
            static T Find(FUNCTION &F, Triplet<T> &x, Triplet<T> &f )
            {
                Wrapper1D<T,T,FUNCTION> FW(F);
                return Find(x,f,FW);
            }


        };
    }

}

#endif
