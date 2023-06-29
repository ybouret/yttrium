//! \file

#ifndef Y_CRC32_Included
#define Y_CRC32_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    struct CRC32
    {
        static uint32_t Run(uint32_t    crc,
                            const void *addr,
                            size_t      size) noexcept;

        static uint32_t Of(const void *addr, size_t size) noexcept;

        template <typename T> static inline
        uint32_t Of(const T &args) noexcept
        {
            return Of(&args,sizeof(T));
        }

    };

}

#endif
