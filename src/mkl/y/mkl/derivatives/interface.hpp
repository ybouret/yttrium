//! \file

#ifndef Y_MKL_Derivatives_Interface_Included
#define Y_MKL_Derivatives_Interface_Included 1

#include "y/object.hpp"
#include "y/counted.hpp"
#include "y/mkl/function-wrapper.hpp"
#include "y/mkl/interval.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Kernel
        {
            class Derivatives : public Object, public Counted
            {
            public:
                static const size_t NTAB = 16;
                virtual ~Derivatives() noexcept;

            protected:
                explicit Derivatives() noexcept;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Derivatives);
            };
        }

        template <typename T>
        class Derivatives : public Kernel::Derivatives
        {
        public:
            typedef Function<T,T> FunctionType;

            explicit Derivatives();
            virtual ~Derivatives() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Derivatives);
            class Code;

            Code *code;
        };

    }
}

#endif

