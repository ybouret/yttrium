
//! \file

#ifndef Y_MKL_Function_Wrapper_Included
#define Y_MKL_Function_Wrapper_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{

    namespace MKL
    {

        template <typename R, typename T>
        class Function
        {
        public:
            Y_ARGS_EXPOSE(T,Type);

            inline virtual ~Function() noexcept {}

            virtual R operator()(ConstType) = 0;


        protected:
            inline explicit Function() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Function);
        };


        template <typename R, typename T, typename FUNCTION>
        class Wrapper : public Function<R,T>
        {
        public:
            typedef typename Function<R,T>::ConstType ConstType;

            inline explicit Wrapper(FUNCTION &f) noexcept : Function<R,T>(), fcn(f) {}
            inline virtual ~Wrapper() noexcept {}

            inline virtual R operator()(ConstType t) { return fcn(t); }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Wrapper);
            FUNCTION &fcn;
        };




    }

}

#endif
