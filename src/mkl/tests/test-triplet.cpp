#include "y/mkl/triplet.hpp"
#include "y/mkl/api.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace MKL;

Y_UTEST(triplet)
{
    Triplet<double>         xx = { 1,2,3 };
    Triplet< XReal<float> > XX = { 1,2,3 };

    std::cerr << xx << std::endl;
    std::cerr << XX << std::endl;

    std::cerr << xx[1] << ", " << xx[2] << ", " << xx[3] << std::endl;

}
Y_UDONE()



