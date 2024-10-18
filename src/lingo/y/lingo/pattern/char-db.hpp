//! \file

#ifndef Y_Lingo_CharDB_Included
#define Y_Lingo_CharDB_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        class CharDB
        {
        public:
            static const unsigned BITS = 256;
            typedef uint8_t       WORD;
            static const unsigned WORD_BITS = sizeof(WORD) * 8;
            static const unsigned WORDS = BITS / WORD_BITS;

        };
    }

}

#endif

