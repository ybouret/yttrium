
#include "y/color/rgb.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(color_rgb)
{

    Color::RGB<uint8_t> rgb8;
    Color::RGB<double>  rgbD(1.1,-1.2,0.1);

    std::cerr << rgb8 << std::endl;
    std::cerr << rgbD << std::endl;

}
Y_UDONE()

