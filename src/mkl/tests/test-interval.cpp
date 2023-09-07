
#include "y/mkl/interval.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace MKL;

Y_UTEST(interval)
{
    Interval<float> I;
    std::cerr << I << std::endl;
    Interval<double> J(-1,3);
    std::cerr << J << std::endl;

    Triplet<double> x = { -1.1,0,3.2 };

    std::cerr << "x   = " << x << std::endl;
    const double slo = J.shiftLower(x);
    const double sup = J.shiftUpper(x);

    std::cerr << "slo = " << slo << std::endl;
    std::cerr << "sup = " << sup << std::endl;
    std::cerr << "x   = " << x   << std::endl;

    
}
Y_UDONE()
