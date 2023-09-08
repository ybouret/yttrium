
//! \file

#ifndef Y_MKL_Function_Included
#define Y_MKL_Function_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{

    namespace MKL
    {
        //______________________________________________________________________
        //
        //
        //
        //! Generic function interface for algorithms
        //
        //
        //______________________________________________________________________
        template <typename R, typename T>
        class Function
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
            // Interface
            //
            //__________________________________________________________________
            virtual R operator()(ParamType) = 0; //!< callable type

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline virtual ~Function() noexcept {} //!< cleanup
        protected:
            inline explicit Function() noexcept {} //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Function);
        };

    }
}

#endif
