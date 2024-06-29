
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
            static const int8_t Table[256];      //!< decoding table, -1 => error
            static const char * const CallSign;  //!< "Base64"

            //! check and decode 1 byte from two chars
            static void _1(uint8_t code[],const char c0, const char c1);

            //! check and decode 2 bytes from three chars
            static void _2(uint8_t code[],const char c0, const char c1, const char c2);

            //! check and decode 3 bytes from four chars
            static void _3(uint8_t code[],const char c0, const char c1, const char c2, const char c3);


        };
    }

}

#endif

