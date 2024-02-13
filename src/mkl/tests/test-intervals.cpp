#include "y/mkl/intervals.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace MKL;

Y_UTEST(intervals)
{

    const size_t      n = 5;
    Intervals<double> domain(n);
    domain[1] = Interval<double>(0,Infinity);
    std::cerr << domain << std::endl;
    const Intervals<double> other(domain);
    std::cerr << other << std::endl;


}
Y_UDONE()
