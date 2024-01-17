
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
                             const Triplet<double> &f)
{
    fp("%.15g %.15g\n",x.a,f.a);
    fp("%.15g %.15g\n",x.b,f.b);
    fp("%.15g %.15g\n",x.c,f.c);
    fp("%.15g %.15g\n",x.a,f.a);
    fp << '\n';
    fp.flush();
}

Y_UTEST(opt_parabolic)
{
    Triplet<double> x = { -0.5, 0, 0.5 };
    Triplet<double> f = { F(x.a), F(x.b), F(x.c) };

    Libc::OutputFile fp("parabolic.dat");
    std::cerr << x << " -> " << f << std::endl;
    saveState(fp,x,f);
    for(int i=0;i<6;++i)
    {
        Parabolic<double>::Step(F,x,f);
        std::cerr << x << " -> " << f << std::endl;
        saveState(fp,x,f);
    }

}
Y_UDONE()
