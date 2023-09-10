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
                typedef typename Named<T>::Equation Equation;
                typedef typename Named<T>::Callback Callback;

                explicit RK45() noexcept;
                explicit RK45(const size_t n);
                virtual ~RK45() noexcept;


                void move(RK45_Step<T> &       step,
                          Writable<T>  &       y,
                          const Readable<T>  & dydx,
                          T                  & x,
                          const T              htry,
                          const T              eps,
                          const Readable<T> &  yscal,
                          T  &                 hdid,
                          T  &                 hnxt,
                          Equation &           drvs,
                          Callback *           cntl);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(RK45);
                class Code;
                Code *code;
            };
        }
    }
}

#endif

