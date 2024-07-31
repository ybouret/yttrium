//! \file


#ifndef Y_Little_Endian_Included
#define Y_Little_Endian_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    //! Little Endian Ops
    struct LittleEndian
    {
        static void Make(uint8_t * const code, const uint8_t  &x) noexcept; //!< x -> code
        static void Make(uint8_t * const code, const uint16_t &x) noexcept; //!< x -> code
        static void Make(uint8_t * const code, const uint32_t &x) noexcept; //!< x -> code
        static void Make(uint8_t * const code, const uint64_t &x) noexcept; //!< x -> code
    };
}

#endif

