
//! \file

#ifndef Y_MKL_Function_WrapperND_Included
#define Y_MKL_Function_WrapperND_Included 1

#include "y/mkl/function.hpp"
#include "y/container/readable.hpp"
#include "y/type/temporary.hpp"


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
        class WrapperND : public Function<R,T>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_DECL(T,Type); //!< aliases

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from a scalar field and its parameters
            inline explicit WrapperND(FUNCTION          &usrFcn,
                                      const Readable<T> &usrVar,
                                      const size_t       varIdx) noexcept :
            Function<R,T>(),
            fcn(usrFcn),
            var(usrVar),
            idx(varIdx)
            {}

            //! cleanup
            inline virtual ~WrapperND() noexcept {} 

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! forward call
            inline virtual R operator()(ParamType t)
            {
                const Temporary<MutableType> tmp( Coerce(var[idx]), t);
                return fcn(var);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(WrapperND);
            FUNCTION          &fcn;
            const Readable<T> &var;
            const size_t       idx;
        };

    }

}

#endif
