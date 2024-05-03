//! \file

#ifndef Y_Ink_Filter_Sobel_Included
#define Y_Ink_Filter_Sobel_Included 1

#include "y/ink/ops/filter/square.hpp"

namespace Yttrium
{
    namespace Ink
    {


        struct Sobel3X
        {
            static const char * const Name;
            static const int          Data[3][3];
        };


        struct Sobel3Y
        {
            static const char * const Name;
            static const int          Data[3][3];
        };

    }

    namespace Ink
    {
        struct Sobel5X
        {
            static const char * const Name;
            static const int          Data[5][5];
        };


        struct Sobel5Y
        {
            static const char * const Name;
            static const int          Data[5][5];
        };

    }

    namespace Ink
    {
        struct Sobel7X
        {
            static const char * const Name;
            static const int          Data[7][7];
        };


        struct Sobel7Y
        {
            static const char * const Name;
            static const int          Data[7][7];
        };

    }




}

#endif

