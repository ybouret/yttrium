
#include "y/color/ramp/black-and-white.hpp"
#include "y/color/rgb/x11.hpp"

namespace Yttrium
{
    namespace Color
    {
        BlackAndWhite:: ~BlackAndWhite() noexcept {}

        static const RampColor bw[2] = { Y_White, Y_Black };

        BlackAndWhite:: BlackAndWhite() noexcept : 
        Gradation( bw, sizeof(bw)/sizeof(bw[0]))
        {
        }
        
    }

}

