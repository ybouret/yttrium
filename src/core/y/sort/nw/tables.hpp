//! \file

#ifndef Y_NetworkSort_Tables_Included
#define Y_NetworkSort_Tables_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace NetworkSort
    {
        struct Table
        {
            const unsigned *index;
            const unsigned  swaps;
        };

        struct Tables
        {
            static const unsigned      MinSize = 2;
            static const unsigned      MaxSize;
            static const Table * const CxxData;
        };




    }
}

#endif
