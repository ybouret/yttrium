
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

    typedef XReal<T> xreal;

    xreal a = 1.2;
    std::cerr << a << std::endl;
    xreal b = 1e4;
    std::cerr << b << std::endl;

    xreal c = (1.2*1e4);
    std::cerr << c << " / " << a*b << std::endl;


}

Y_UTEST(mkl_xreal)
{
    checkXREAL<float>();
    checkXREAL<double>();
    checkXREAL<long double>();

}
Y_UDONE()

