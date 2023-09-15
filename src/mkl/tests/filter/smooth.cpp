#include "y/mkl/filter/selector.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/random/bits.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"

#include <cmath>

using namespace Yttrium;

Y_UTEST(filter_smooth)
{

    Random::Rand ran;

    Vector<double> X,Y;


    {
        const size_t np=100;
        Libc::OutputFile fp("smooth-data.dat");
        double xx = 0;
        for(size_t i=0;i<=np;++i)
        {
            xx += 0.05 + 0.05 * ran.to<double>();
            X << xx;
            Y << cos(xx);
            fp("%.15g %.15g\n", X.tail(), Y.tail());
        }

    }

    const double   xmin  = 0;
    const double   xmax  = X.tail()*1.01;
    Vector<double> xs, ys;


    MKL::Antelope::Add<double> xadd;

    {
        Libc::OutputFile fp("smooth.dat");
        const size_t NP = 10;
        for(size_t i=0;i<=NP;++i)
        {
            const double xx = xmin + i*(xmax-xmin)/double(NP);
            MKL::Selector::Fill(xx, 0.0, 0.0, xs, ys, X, Y);
            fp("%.15g %.15g\n", xx, xadd.average(ys) );
        }
    }








}
Y_UDONE()

