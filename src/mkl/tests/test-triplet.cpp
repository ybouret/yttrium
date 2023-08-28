#include "y/mkl/triplet.hpp"
#include "y/mkl/api.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace MKL;

Y_UTEST(triplet)
{
    Triplet<double>         xx = { 3,1,2 };
    Triplet< XReal<float> > XX = { 3,1,2 };

    std::cerr << xx << std::endl;
    std::cerr << XX << std::endl;

    std::cerr << xx[1] << ", " << xx[2] << ", " << xx[3] << std::endl;

    xx.makeIncreasing();
    Y_CHECK(xx.isIncreasing());
    std::cerr << xx << std::endl;

    XX.makeIncreasing();
    Y_CHECK(XX.isIncreasing());
    std::cerr << XX << std::endl;

    xx.a = 3; xx.b = 1; xx.c = 2;
    XX.a = 1; XX.b = 2; XX.c = 3;

    xx.makeIncreasing(XX);
    Y_CHECK(xx.isIncreasing());
    std::cerr << xx << ", " << XX << std::endl;

}
Y_UDONE()



