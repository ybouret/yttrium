
#include "y/mkl/opt/parabolic.hpp"
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

Y_UTEST(opt_parabolic)
{
    Triplet<double> x = { -0.5, 0, 0.5 };
    Triplet<double> f = { F(x.a), F(x.b), F(x.c) };

    std::cerr << x << " -> " << f << std::endl;
    for(int i=0;i<6;++i)
    {
        Parabolic<double>::Step(F,x,f);
        std::cerr << x << " -> " << f << std::endl;
    }

}
Y_UDONE()
