//! \file

#ifndef Y_Apex_Types_Included
#define Y_Apex_Types_Included 1

#include "y/calculus/align.hpp"

namespace Yttrium
{
    namespace Apex
    {

        typedef uint64_t  natural_t;
        typedef int64_t   integer_t;

        enum Plan
        {
            Plan1=0,
            Plan2=1,
            Plan4=2,
            Plan8=3
        };

#define Y_Apex_Bytes_For(BITS) ( (Y_ALIGN_ON(8,BITS)) >> 3 )

    }
}

#endif
