


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

    static inline double dFcn(const double x)
    {
        return 0.9 * sin(0.9 * x - 0.1);
    }
}


Y_UTEST(drvs_1d)
{
    Derivatives<double> drvs;
    drvs.Verbose =  true;
    Fcn F = { 0 };
    const Interval<double> I(0,1);
    drvs.compute(F,0.2,0.5,I);

    //drvs.compute(F,0.99,0.5,I);

}
Y_UDONE()
