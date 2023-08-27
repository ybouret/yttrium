//! \file

#ifndef Y_MKL_ODE_RK4_Included
#define Y_MKL_ODE_RK4_Included 1

#include "y/mkl/ode/types.hpp"
#include <iostream>

namespace Yttrium
{

    namespace MKL
    {

        namespace ODE
        {
            template <typename T>
            class RK4
            {
            public:
                typedef typename Field<T>::Equation Equation;

                explicit RK4() noexcept;
                explicit RK4(const size_t dims);
                virtual ~RK4() noexcept;

                void operator()(Writable<T> &Y,
                                Equation    &eqs,
                                const T      t0,
                                const T      t1);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(RK4);
                class Code;
                Code *code;
            };
        }

    }

}

#endif

