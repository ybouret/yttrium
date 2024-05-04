

#include "y/ink/ops/filter/sobel.hpp"

namespace Yttrium
{
    namespace Ink
    {

        namespace Crux
        {
            const char * const Sobel3X::Name = "Sobel3X";
            const int          Sobel3X::Data[3][3] = {
                {-1,0,1},
                {-2,0,2},
                {-1,0,1}
            };

            const char * const Sobel3Y::Name = "Sobel3Y";
            const int          Sobel3Y::Data[3][3] = {
                {-1,-2,-1},
                {0,0,0},
                {1,2,1}
            };

            const char * const Sobel3:: Name = "Sobel3";



            const char * const Sobel5X::Name = "Sobel5X";
            const int          Sobel5X::Data[5][5] = {
                {  -5,  -4, 0,  4,  5 },
                {  -8, -10, 0, 10,  8 },
                { -10, -20, 0, 20, 10 },
                {  -8, -10, 0, 10,  8 },
                {  -5,  -4, 0,  4,  5 },
            };

            const char * const Sobel5Y::Name = "Sobel5Y";
            const int          Sobel5Y::Data[5][5] = {
                { -5,  -8, -10,  -8, -5 },
                { -4, -10, -20, -10, -4 },
                {  0,   0,   0,   0,  0 },
                {  4,  10,  20,  10,  4 },
                {  5,   8,  10,   8,  5 },
            };

            const char * const Sobel5:: Name = "Sobel5";


            const char * const Sobel7X::Name = "Sobel7X";
            const int          Sobel7X::Data[7][7] = {
                {  -780,  -720,  -468, 0,  468,  720,  780 },
                { -1080, -1170,  -936, 0,  936, 1170, 1080 },
                { -1404, -1872, -2340, 0, 2340, 1872, 1404 },
                { -1560, -2340, -4680, 0, 4680, 2340, 1560 },
                { -1404, -1872, -2340, 0, 2340, 1872, 1404 },
                { -1080, -1170,  -936, 0,  936, 1170, 1080 },
                {  -780,  -720,  -468, 0,  468,  720,  780 },
            };

            const char * const Sobel7Y::Name = "Sobel7Y";
            const int          Sobel7Y::Data[7][7] = {
                { -780, -1080, -1404, -1560, -1404, -1080, -780 },
                { -720, -1170, -1872, -2340, -1872, -1170, -720 },
                { -468,  -936, -2340, -4680, -2340,  -936, -468 },
                {    0,     0,     0,     0,     0,     0,    0 },
                {  468,   936,  2340,  4680,  2340,   936,  468 },
                {  720,  1170,  1872,  2340,  1872,  1170,  720 },
                {  780,  1080,  1404,  1560,  1404,  1080,  780 }
            };

            const char * const Sobel7:: Name = "Sobel7";

        }
    }

}

