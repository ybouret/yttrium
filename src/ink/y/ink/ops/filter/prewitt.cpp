
#include "y/ink/ops/filter/prewitt.hpp"

namespace Yttrium
{
    namespace Ink
    {

        namespace Crux
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

            const char * const Prewitt3::Name = "Prewitt3";

            const char * const Prewitt5X::Name = "Prewitt5X";
            const int          Prewitt5X::Data[5][5] = {
                {-2, -1,0,1, 2},
                {-2, -1,0,1, 2},
                {-2, -1,0,1, 2},
                {-2, -1,0,1, 2},
                {-2, -1,0,1, 2},
            };

            const char * const Prewitt5Y::Name = "Prewitt5Y";
            const int          Prewitt5Y::Data[5][5] = {
                {-2, -2, -2, -2, -2 },
                {-1, -1, -1, -1, -1 },
                { 0,  0,  0,  0,  0 },
                { 1,  1,  1,  1,  1 },
                { 2,  2,  2,  2,  2 }
            };

            const char * const Prewitt5::Name = "Prewitt5";


            const char * const Prewitt7X::Name = "Prewitt7X";
            const int          Prewitt7X::Data[7][7] = {
                {-3, -2, -1,0,1, 2, 3},
                {-3, -2, -1,0,1, 2, 3},
                {-3, -2, -1,0,1, 2, 3},
                {-3, -2, -1,0,1, 2, 3},
                {-3, -2, -1,0,1, 2, 3},
                {-3, -2, -1,0,1, 2, 3},
                {-3, -2, -1,0,1, 2, 3}
            };

            const char * const Prewitt7Y::Name = "Prewitt7Y";
            const int          Prewitt7Y::Data[7][7] = {
                {-3, -3, -3, -3, -3, -3, -3 },
                {-2, -2, -2, -2, -2, -2, -2 },
                {-1, -1, -1, -1, -1, -1, -1 },
                { 0,  0,  0,  0,  0,  0,  0 },
                { 1,  1,  1,  1,  1,  1,  1 },
                { 2,  2,  2,  2,  2,  2,  2 },
                { 3,  3,  3,  3,  3,  3,  3 }
            };
            
            const char * const Prewitt7::Name = "Prewitt7";

        }
    }

}

