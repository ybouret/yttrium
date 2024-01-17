
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

static inline void saveState(OutputStream          &fp,
                             const Triplet<double> &x,
                             const Triplet<double> &f,
                             const unsigned         i)
{
    fp("%.15g %.15g %d\n",x.a,f.a,i);
    fp("%.15g %.15g %d\n",x.b,f.b,i);
    fp("%.15g %.15g %d\n",x.c,f.c,i);
    fp("%.15g %.15g %d\n",x.a,f.a,i);
    fp << '\n';
    fp.flush();
}

Y_UTEST(opt_parabolic)
{
    Triplet<double> x = { -0.5, 0, 0.5 };
    Triplet<double> f = { F(x.a), F(x.b), F(x.c) };

    Libc::OutputFile fp("parabolic.dat");
    std::cerr << x << " -> " << f << std::endl;
    saveState(fp,x,f,0);
    for(int i=1;i<=6;++i)
    {
        Parabolic<double>::Step(F,x,f);
        std::cerr << x << " -> " << f << std::endl;
        saveState(fp,x,f,i);
    }

}
Y_UDONE()
