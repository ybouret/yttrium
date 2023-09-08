//! \file

#ifndef Y_MKL_Function_Wrapper1D_Included
#define Y_MKL_Function_Wrapper1D_Included 1

#include "y/mkl/function.hpp"

namespace Yttrium
{

    namespace MKL
    {

        //______________________________________________________________________
        //
        //
        //
        //! Wrapper for any User's function
        //
        //
        //______________________________________________________________________
        template <typename R, typename T, typename FUNCTION>
        class Wrapper1D : public Function<R,T>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_DECL(T,Type); //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit Wrapper1D(FUNCTION &f) noexcept : Function<R,T>(), fcn(f) {} //!< setup
            inline virtual ~Wrapper1D()            noexcept                           {} //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! forward call
            inline virtual R operator()(ParamType t) { return fcn(t); }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Wrapper1D);
            FUNCTION &fcn;
        };

        



    }

}

#endif
