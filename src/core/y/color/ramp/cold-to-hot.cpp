

#include "y/color/ramp/cold-to-hot.hpp"
#include "y/color/rgb/x11.hpp"

namespace Yttrium
{
    namespace Color
    {
        ColdToHot:: ~ColdToHot() noexcept {}

        static const RampColor cr[] = { Y_Blue, Y_Cyan, Y_Green, Y_Yellow, Y_Red };

        ColdToHot:: ColdToHot() noexcept :
        Gradation( Y_COLOR_GRADATION(cr) )
        {
        }

    }

}

