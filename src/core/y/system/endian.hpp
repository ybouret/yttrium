//! \file

#ifndef Y_System_Endian_Included
#define Y_System_Endian_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    struct Endianness
    {
        enum Type
        {
            Unknown,
            Big,
            Little,
            BigWord,   /* Middle-endian, Honeywell 316 style */
            LittleWord /* Middle-endian, PDP-11 style */
        };

        static const char * Text(const Type) noexcept;

        static Type Detect() noexcept;

    };

}

#endif
