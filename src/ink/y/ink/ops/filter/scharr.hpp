
//! \file

#ifndef Y_Ink_Filter_Scharr_Included
#define Y_Ink_Filter_Scharr_Included 1

#include "y/ink/ops/filter/square.hpp"

namespace Yttrium
{
    namespace Ink
    {


        struct Scharr3X
        {
            static const char * const Name;
            static const int          Data[3][3];
        };


        struct Scharr3Y
        {
            static const char * const Name;
            static const int          Data[3][3];
        };

    }

    namespace Ink
    {
        struct Scharr5X
        {
            static const char * const Name;
            static const int          Data[5][5];
        };


        struct Scharr5Y
        {
            static const char * const Name;
            static const int          Data[5][5];
        };

    }

}

#endif

