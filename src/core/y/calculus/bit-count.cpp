#include "y/calculus/bit-count.hpp"

namespace Yttrium
{
    const uint8_t BitCount::Table[256] =
    {
        0x0,  0x1,  0x2,  0x2,  0x3,  0x3,  0x3,  0x3,  0x4,  0x4,  0x4,  0x4,  0x4,  0x4,  0x4,  0x4,
        0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,
        0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,
        0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,  0x6,
        0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,
        0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,
        0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,
        0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,  0x7,
        0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,
        0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,
        0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,
        0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,
        0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,
        0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,
        0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,
        0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8
    };



}

