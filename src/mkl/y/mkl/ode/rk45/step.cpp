
#include "y/mkl/ode/rk45/step.hpp"
#include "y/mkl/xreal.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace ODE
        {

            namespace RK45
            {

#define real_t float
#include "step.hxx"

#undef  real_t
#define real_t double
#include "step.hxx"

#undef  real_t
#define real_t long double
#include "step.hxx"

#undef  real_t
#define real_t XReal<float>
#include "step.hxx"

#undef  real_t
#define real_t XReal<double>
#include "step.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "step.hxx"
                
            }

        }

    }

}

