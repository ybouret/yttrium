


#include "y/mkl/derivatives/interface.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/xreal.hpp"
#include "y/mkl/triplet.hpp"
#include <cmath>

using namespace Yttrium;
using namespace MKL;


namespace
{
    struct Fcn
    {
        size_t calls;

        inline double operator()(double x)
        {
            ++calls;
            return  0.7-cos(0.9*x-0.1);
        }

    };
}


Y_UTEST(drvs_1d)
{
    Derivatives<double> drvs;
    Fcn F = { 0 };
    std::cerr << "calls=" << F.calls << std::endl;
    double x = 0.5;
    drvs.eval(F,x-0.6,x,x+0.7);
    std::cerr << "calls=" << F.calls << std::endl;


    const Interval<double> I(0,1);
    std::cerr << "I=" << I << std::endl;

    Triplet<double> xx;
    double length = 2;
    drvs.setMetrics(xx,0.1, length, I);

    std::cerr << "xx=" << xx << std::endl;
    


}
Y_UDONE()
