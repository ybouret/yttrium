#include "y/mkl/ode/rk45/scheme.hpp"
#include "y/mkl/ode/rk45/cash-karp.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    template <typename T> static inline
    void testScheme(Random::Bits & )
    {
        ODE::RK45::Scheme<ODE::RK45::CashKarp,T> scheme;
        
    }
}

Y_UTEST(ode_scheme45)
{
    Random::Rand ran;

    testScheme<float>(ran);
    testScheme<double>(ran);
    testScheme<long double>(ran);


}
Y_UDONE()
