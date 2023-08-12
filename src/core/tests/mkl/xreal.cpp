
#include "y/mkl/xreal.hpp"
#include "y/mkl/numeric.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"
#include "y/random/bits.hpp"

#include <cmath>

using namespace Yttrium;
using namespace MKL;

template <typename T> static inline T genReal(Random::Bits &ran)
{
    const int    exponent = ran.in<int>(Numeric<T>::MIN_EXP+1,Numeric<T>::MAX_EXP-1);
    const T      mantissa = ran.in<T>(0.5,0.999);
    const T      r        = std::ldexp(mantissa,exponent);
    return ran.choice() ? -r : +r;

}
template <typename T>
static inline void checkXREAL(Random::Bits &ran)
{
    typedef XReal<T> xreal;
    typedef T        real;

    {
        const real a = std::sqrt(std::abs(genReal<real>(ran)));
        const real b = std::sqrt(std::abs(genReal<real>(ran)));
        const real c = a*b;
        const xreal A=a;
        const xreal B=b;
        const xreal C=A*B;
        std::cerr << c << " -> " << C << std::endl;
    }



}

Y_UTEST(mkl_xreal)
{
    Random::Rand ran;
    checkXREAL<float>(ran);
    checkXREAL<double>(ran);
    checkXREAL<long double>(ran);

}
Y_UDONE()

