
//! file

#ifndef Y_ODE_Integrator_Included
#define Y_ODE_Integrator_Included 1

#include "y/mkl/ode/rk45.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace ODE
        {


            template <typename T>
            class Integrator
            {
            public:
                typedef typename Named<T>::Equation Equation;
                typedef typename Named<T>::Callback Callback;

                explicit Integrator(const T userEps) noexcept;
                explicit Integrator(const T userEps,const size_t n);
                virtual ~Integrator() noexcept;

                void run(Writable<T> &ystart,
                         const T      x1,
                         const T      x2,
                         T           &hini,
                         Equation    &drvs,
                         Callback    *cntl,
                         Actuator<T> &actuator);

                T eps;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Integrator);
                class Code;
                Code *code;
            };

        }

    }

}

#endif
