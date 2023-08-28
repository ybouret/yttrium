
//! \file

#ifndef Y_MKL_Function_Wrapper_Included
#define Y_MKL_Function_Wrapper_Included 1

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
            Y_ARGS_EXPOSE(T,Type); //!< aliases

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual R operator()(ConstType) = 0; //!< callable type

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


        //______________________________________________________________________
        //
        //
        //
        //! Wrapper for any User's function
        //
        //
        //______________________________________________________________________
        template <typename R, typename T, typename FUNCTION>
        class Wrapper : public Function<R,T>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef typename Function<R,T>::ConstType ConstType; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit Wrapper(FUNCTION &f) noexcept : Function<R,T>(), fcn(f) {} //!< setup
            inline virtual ~Wrapper()            noexcept                           {} //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! forward call
            inline virtual R operator()(ConstType t) { return fcn(t); }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Wrapper);
            FUNCTION &fcn;
        };




    }

}

#endif
