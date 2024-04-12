

#include "y/color/rgba.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(color_rgba)
{

    Color::RGBA<uint8_t> rgb8;
    Color::RGBA<double>  rgbD(1.1,-1.2,0.1);

    std::cerr << rgb8 << std::endl;
    std::cerr << rgbD << std::endl;


}
Y_UDONE()

