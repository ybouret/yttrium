#include "y/mkl/derivatives/interface.hpp"
#include "y/utest/run.hpp"
#include "y/container/cxx-array.hpp"
#include "y/memory/allocator/dyadic.hpp"

#include <cmath>

using namespace Yttrium;
using namespace MKL;

namespace
{
    struct Field
    {

        inline double compute(const Readable<double> &X)
        {
            return (X[1]+2*X[2])/sqrt(1+X[1]*X[1]+X[2]*X[2]);
        }

        inline double operator()(const Readable<double> &X)
        {
            return compute(X);
        }

    };
}

Y_UTEST(drvsND)
{
    Derivatives<double> drvs;

    Field F;

    CxxArray<double,Memory::Dyadic>           H(2,0.02);
    CxxArray<Interval<double>,Memory::Dyadic> I(2);

    std::cerr << "H=" << H << std::endl;
    std::cerr << "I=" << I << std::endl;
    I[2] = Interval<double>(-1,1);
    std::cerr << "I=" << I << std::endl;

    CxxArray<double,Memory::Dyadic> dFdX(2);
    CxxArray<double,Memory::Dyadic> X(2);
    X[1]=0.1;
    X[2]=0.99;
    drvs(dFdX,F,X,H,I);
    std::cerr << "dFdX=" << dFdX << std::endl;

}
Y_UDONE()
