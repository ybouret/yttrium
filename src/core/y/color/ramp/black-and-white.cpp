
#include "y/color/ramp/black-and-white.hpp"
#include "y/color/rgb/x11.hpp"

namespace Yttrium
{
    namespace Color
    {
        BlackAndWhite:: ~BlackAndWhite() noexcept {}

        static const RampColor bw[] = { Y_Black, Y_White };

        BlackAndWhite:: BlackAndWhite() noexcept : 
        Gradation( Y_COLOR_GRADATION(bw) )
        {
        }
        
    }

}

