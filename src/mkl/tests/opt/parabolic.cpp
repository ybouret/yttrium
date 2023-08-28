
#include "y/mkl/opt/parabolic.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"
#include "y/mkl/xreal.hpp"
#include <cmath>

using namespace Yttrium;
using namespace MKL;



double Sin(double x)
{
    return sin(x);
}

Y_UTEST(opt_parabolic)
{
    Triplet<double> x,f;

    Parabolic<double>::Step(Sin,x,f);

}
Y_UDONE()
