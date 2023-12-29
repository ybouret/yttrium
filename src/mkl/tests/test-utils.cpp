
#include "y/mkl/utils.hpp"
#include "y/mkl/numeric.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace MKL;

Y_UTEST(utils)
{
    {
        const float one = 1;
        const float eps = Numeric<float>::EPSILON / 2;
        Y_CHECK(IsUnchanged(one,eps) || Die("float") );
    }

    {
        const double one = 1;
        const double eps = Numeric<double>::EPSILON / 2;
        Y_CHECK(IsUnchanged(one,eps) || Die("double") );
    }

    {
        const long double one = 1;
        const long double eps = Numeric<long double>::EPSILON / 2;
        Y_CHECK(IsUnchanged(one,eps) || Die("long double") );
    }
}
Y_UDONE()
