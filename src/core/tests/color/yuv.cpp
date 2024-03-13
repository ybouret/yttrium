

#include "y/color/yuv.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(color_yuv)
{

    Color::YUV<double>  yuv(1.1,-1.2,0.1);

    std::cerr << yuv << std::endl;


}
Y_UDONE()

