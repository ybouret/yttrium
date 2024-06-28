
//! \file

#ifndef Y_Base64_Decode_Included
#define Y_Base64_Decode_Included 1

#include "y/config/starting.hpp"


namespace Yttrium
{

    namespace Base64
    {
        //______________________________________________________________________
        //
        //
        //
        //! Base64 Decode operations
        //
        //
        //______________________________________________________________________
        struct Decode
        {
            static const int8_t Table[256];
            static const char * const CallSign;
            
            static void _1(uint8_t code[],const char c0, const char c1);

        };
    }

}

#endif

