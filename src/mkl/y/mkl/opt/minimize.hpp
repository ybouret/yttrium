
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

            //! prolog befor locationg a minimz
            enum Prolog
            {
                Direct, //!< local minimum estimated by user
                Inside, //!< will bracket inside x.a:x.c
                Expand  //!< will bracket expand x.a:x.b
            };
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
            static T Locate(const Minimizing::Prolog prolog, Triplet<T> &x, Triplet<T> &f, FunctionType &F);
            template <typename FUNCTION>  inline

            //! wrapper
            static T Locate(const Minimizing::Prolog prolog, FUNCTION &F, Triplet<T> &x, Triplet<T> &f)
            {
                Wrapper1D<T,T,FUNCTION> FW(F);
                return Locate(prolog,x,f,FW);
            }


        };
    }

}

#endif
