
//! \file

#ifndef Y_Text_Convert_Included
#define Y_Text_Convert_Included 1

#include "y/config/starting.hpp"


namespace Yttrium
{


    struct Convert
    {
        static uint64_t ToU64(const char *msg, size_t len, const char *ctx=0);
        
    };

}

#endif


