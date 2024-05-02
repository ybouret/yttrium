
#include "y/color/channels.hpp"
#include "y/color/yuv.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Color;

Y_UTEST(color_channels)
{
    Y_USHOW(Channels<float>::NCH);
    Y_USHOW(Channels< RGB<uint8_t> >::NCH);
    Y_USHOW(Channels< YUV<float>   >::NCH);

}
Y_UDONE()


