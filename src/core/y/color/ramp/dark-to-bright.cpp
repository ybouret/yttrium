

#include "y/color/ramp/dark-to-bright.hpp"
#include "y/color/rgb/x11.hpp"

namespace Yttrium
{
    namespace Color
    {
        DarkToBright:: ~DarkToBright() noexcept {}

        static const RampColor cr[] = { Y_Black, Y_Blue, Y_Cyan, Y_Green, Y_Yellow, Y_Red,  Y_White };

        DarkToBright:: DarkToBright() noexcept :
        Gradation( Y_COLOR_GRADATION(cr) )
        {
        }

    }

}

