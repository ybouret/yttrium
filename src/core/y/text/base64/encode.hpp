
//! \file

#ifndef Y_Base64_Encode_Included
#define Y_Base64_Encode_Included 1

#include "y/config/starting.hpp"


namespace Yttrium
{

    namespace Base64
    {
        struct Encode
        {
            static const char Table[64];
            static const char Padding = '=';

            static void   ShowInfo();

            //! 1 code => 2 chars, 4 chars with padding
            static size_t _1(char * output, const uint8_t code, const bool pad) noexcept;

            //! 2 code2 => 3 chars, 4 chars with padding
            static size_t _2(char * ouput, const uint8_t c0, const uint8_t c1, const bool pad) noexcept;

            //! 3 codes => 4 chaers
            static size_t _3(char *ouput, const uint8_t c0, const uint8_t c1, const uint8_t c2) noexcept;


        };
    }

}


#endif
