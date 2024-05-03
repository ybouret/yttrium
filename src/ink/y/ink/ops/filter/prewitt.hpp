
//! \file

#ifndef Y_Ink_Filter_Prewitt_Included
#define Y_Ink_Filter_Prewitt_Included 1

#include "y/ink/ops/filter/square.hpp"

namespace Yttrium
{
    namespace Ink
    {


        struct Prewitt3X
        {
            static const char * const Name;
            static const int          Data[3][3];
        };


        struct Prewitt3Y
        {
            static const char * const Name;
            static const int          Data[3][3];
        };

        struct Prewitt5X
        {
            static const char * const Name;
            static const int          Data[5][5];
        };


        struct Prewitt5Y
        {
            static const char * const Name;
            static const int          Data[5][5];
        };




    }

}

#endif

