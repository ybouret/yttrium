//! file

#ifndef Y_ODE_RK45_Included
#define Y_ODE_RK45_Included 1

#include "y/mkl/ode/cash-karp.hpp"
#include "y/mkl/ode/actuator.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace ODE
        {

          
            template <typename T>
            class RK45 : public Actuator<T>
            {
            public:
                typedef typename Named<T>::Equation   Equation;
                typedef typename Named<T>::Callback   Callback;
                typedef typename RK45_Step<T>::Handle StepType;

                explicit RK45(const StepType &s) noexcept;
                explicit RK45(const StepType &s, const size_t n);
                virtual ~RK45() noexcept;


                virtual void move(Writable<T>  &       y,
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
                class    Code;
                StepType step;
                Code *   code;
            };
        }
    }
}

#endif

