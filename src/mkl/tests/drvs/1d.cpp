
#include "y/mkl/derivatives/interface.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/xreal.hpp"
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

    static inline double Cos(double x) { return cos(x); }
}


Y_UTEST(drvs1D)
{
    Derivatives<double> drvs;
    Fcn F = { 0 };
    const Interval<double> I(0,4);
    std::cerr << drvs(F,0.2,0.8,I) << std::endl;
    std::cerr << "\t\texpected: " << dFcn(0.2) << std::endl;

    std::cerr << drvs(F,0.2,0.1,I) << std::endl;
    std::cerr << "\t\texpected: " << dFcn(0.2) << std::endl;

    std::cerr << drvs(F,3.99,0.5,I) << std::endl;
    std::cerr << "\t\texpected: " << dFcn(3.99) << std::endl;

    const Interval<double> R;
    std::cerr << drvs(Cos,0.0,0.1,R) << std::endl;
    std::cerr << "\t\texpected: 0" << std::endl;

    std::cerr << drvs(Cos,0.0,0.1,I) << std::endl;
    std::cerr << "\t\texpected: 0" << std::endl;



}
Y_UDONE()
