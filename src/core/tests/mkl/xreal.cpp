
#include "y/mkl/xreal.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"
#include <cmath>

using namespace Yttrium;

template <typename T>
static inline void checkXREAL()
{
    const T zero = 0;
    int     zexp = 0;
    const T zm = std::frexp(zero,&zexp);
    std::cerr << "zero: mantissa=" << zm << ", exponent=" << zexp << std::endl;
}

Y_UTEST(mkl_xreal)
{
    checkXREAL<float>();
    checkXREAL<double>();
    checkXREAL<long double>();

}
Y_UDONE()

