
#include "y/mkl/v3d.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"
#include "y/random/in2d.hpp"

using namespace Yttrium;

Y_UTEST(mkl_vtx)
{
    Random::Rand ran;
    
    V2D<float> v(0,0);
    Random::In2D::Circle(v.x, v.y, ran);
}
Y_UDONE()
