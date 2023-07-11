
// \file

#ifndef Y_Random_Fill_Included
#define Y_Random_Fill_Included 1

#include "y/random/bits.hpp"

namespace Yttrium
{
    namespace Random
    {

        struct Fill
        {
            static void Block(void *blockAddr,    size_t blockSize, Bits &ran) noexcept;
            static void BlockGTZ(void *blockAddr, size_t blockSize, Bits &ran) noexcept;
        };

    }

}

#endif

