

#include "y/mkl/opt/minimize.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"
#include "y/mkl/xreal.hpp"
#include <cmath>

using namespace Yttrium;
using namespace MKL;



static inline double F(double x)
{
    return  0.7-cos(0.9*x-0.1);
}



Y_UTEST(opt_minimize)
{
    Triplet<double> x = { -0.5, 0, 0.5 };
    Triplet<double> f = { F(x.a), F(x.b), F(x.c) };

    Minimizing::Verbose = true;


    const double x_opt = Minimize<double>::Locate( Minimizing::Inside, F, x, f);
    std::cerr << "x_opt=" << x_opt << std::endl;

}
Y_UDONE()
