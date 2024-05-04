#include "y/ink/ops/filter/scharr.hpp"

namespace Yttrium
{
    namespace Ink
    {

        namespace Crux
        {
            const char * const Scharr3X::Name = "Scharr3X";
            const int          Scharr3X::Data[3][3] = {
                {-1,0,1},
                {-3,0,3},
                {-1,0,1}
            };
            
            const char * const Scharr3Y::Name = "Scharr3Y";
            const int          Scharr3Y::Data[3][3] = {
                {-1,-3,-1},
                {0,0,0},
                {1,3,1}
            };
            
            const char * const Scharr3:: Name = "Scharr3";

            const char * const Scharr5X::Name = "Scharr5X";
            const int          Scharr5X::Data[5][5] = {
                {  -1,  -1, 0,  1,  1 },
                {  -2,  -2, 0,  2,  2 },
                {  -3,  -6, 0,  6,  3 },
                {  -2,  -2, 0,  2,  2 },
                {  -1,  -1, 0,  1,  1 },
            };
            
            const char * const Scharr5Y::Name = "Scharr5Y";
            const int          Scharr5Y::Data[5][5] = {
                { -1, -2, -3, -2, -1 },
                { -1, -2, -6, -2, -1 },
                {  0,  0,  0,  0,  0 },
                {  1,  2,  6,  2,  1 },
                {  1,  2,  3,  2,  1 },
            };

            const char * const Scharr5:: Name = "Scharr5";


        }
    }




}

