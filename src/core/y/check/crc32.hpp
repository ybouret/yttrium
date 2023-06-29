//! \file

#ifndef Y_CRC32_Included
#define Y_CRC32_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Memory
    {
        struct CRC32
        {
            static uint32_t Run(uint32_t    crc,
                               const void *addr,
                                size_t      size) noexcept;

            static uint32_t Of(const void *addr, size_t size) noexcept;
        };
    }
}

#endif
