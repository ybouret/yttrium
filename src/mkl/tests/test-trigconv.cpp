#include "y/mkl/trigconv.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace MKL;

Y_UTEST(trigconv)
{
    for(int deg=0;deg<=360;deg+=10)
    {
        const double rad = TrigConv::DegToRad<double>(deg);
        const double old = TrigConv::RadToDeg<double>(rad);
        std::cerr << deg << " -> " << rad << " -> " << old;
        std::cerr << std::endl;
    }
}
Y_UDONE()
