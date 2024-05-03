
#include "y/ink/ops/filter/prewitt.hpp"

namespace Yttrium
{
    namespace Ink
    {

        const char * const Prewitt3X::Name = "Prewitt3X";
        const int          Prewitt3X::Data[3][3] = {
            {-1,0,1},
            {-1,0,1},
            {-1,0,1}
        };

        const char * const Prewitt3Y::Name = "Prewitt3Y";
        const int          Prewitt3Y::Data[3][3] = {
            {-1,-1,-1},
            {0,0,0},
            {1,1,1}
        };

    }

    namespace Ink
    {

        const char * const Prewitt5X::Name = "Prewitt5X";
        const int          Prewitt5X::Data[5][5] = {
            {-2, -1,0,1, 2},
            {-2, -1,0,1, 2},
            {-2, -1,0,1, 2},
            {-2, -1,0,1, 2},
            {-2, -1,0,1, 2},
        };

        const char * const Prewitt5Y::Name = "Prewitt3Y";
        const int          Prewitt5Y::Data[5][5] = {

        };

    }

}

