
#include "y/color/rgb.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(color_rgb)
{

    Color::RGB<uint8_t> rgb;
    std::cerr << rgb << std::endl;

}
Y_UDONE()

