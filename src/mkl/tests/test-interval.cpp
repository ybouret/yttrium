
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

    
}
Y_UDONE()
