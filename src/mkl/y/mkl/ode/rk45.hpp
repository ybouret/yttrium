//! file

#ifndef Y_ODE_RK45_Included
#define Y_ODE_RK45_Included 1

#include "y/mkl/ode/cash-karp.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace ODE
        {
            template <typename T>
            class RK45
            {
            public:
                explicit RK45() noexcept;
                explicit RK45(const size_t n);
                virtual ~RK45() noexcept;

                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(RK45);
                class Code;
                Code *code;
            };
        }
    }
}

#endif

