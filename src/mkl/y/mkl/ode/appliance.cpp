
#include "y/mkl/ode/appliance.hpp"
#include "y/mkl/xreal.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace ODE
        {
            
#define real_t float
#include "appliance.hxx"

#undef  real_t
#define real_t double
#include "appliance.hxx"

#undef  real_t
#define real_t long double
#include "appliance.hxx"

#undef  real_t
#define real_t XReal<float>
#include "appliance.hxx"

#undef  real_t
#define real_t XReal<double>
#include "appliance.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "appliance.hxx"

        }

    }

}

