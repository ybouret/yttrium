#include "y/random/bits.hpp"
#include "y/mkl/statistics/metrics.hpp"
#include "y/utest/run.hpp"
#include "y/type/utils.hpp"
#include "y/sequence/vector.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace MKL;


Y_UTEST(strain_sim)
{
    Random::Rand   ran;
    Vector<double> data;
    Antelope::Add<double> xadd;

    double gamma  = 1.0;
    double lambda = 2.0;

    data << gamma * Squared( lambda * ran.symm<double>() );

    OutputFile fp("strain-sim.dat");

    for(unsigned n=2;n<=10;++n)
    {
        data << gamma * Squared( lambda * ran.symm<double>() );
        const double ave = Statistics::Average(data,xadd);
        const double sig = Statistics::StandardDeviation(data,xadd,ave);
        const double skw = Statistics::Skewness(data,xadd,ave,sig);
        const double krt = Statistics::Kurtosis(data,xadd,ave,sig);
        fp("%u %.15g %.15g %.15g %.15g\n", n, ave, sig, skw, krt);
    }






}
Y_UDONE()
