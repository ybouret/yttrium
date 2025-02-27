
//! \file

#ifndef Y_Apex_IO_Included
#define Y_Apex_IO_Included 1

#include "y/apex/natural.hpp"
#include "y/stream/bits.hpp"

namespace Yttrium
{

    namespace Apex
    {

        struct IOBits
        {
            static void Store(StreamBits &io, const Natural &n);

        };

    }

}

#endif

