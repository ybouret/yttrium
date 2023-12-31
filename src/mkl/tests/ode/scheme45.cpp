#include "y/mkl/ode/rk45/scheme.hpp"
#include "y/mkl/ode/rk45/cash-karp.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    template <typename T> static inline
    void testScheme()
    {
        ODE::RK45::Scheme<ODE::RK45::CashKarp,T> scheme;
        
    }
}

Y_UTEST(ode_scheme45)
{

}
Y_UDONE()
