//! \file

#ifndef Y_CRC32_Included
#define Y_CRC32_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! basic CRC32 algorithm
    //
    //
    //__________________________________________________________________________
    struct CRC32
    {
        //______________________________________________________________________
        //
        //! updated current crc from addr[size]
        //______________________________________________________________________
        static uint32_t Run(uint32_t    crc,
                            const void *addr,
                            size_t      size) noexcept;

        //______________________________________________________________________
        //
        //! crc32 of a block
        //______________________________________________________________________
        static uint32_t Of(const void *addr, size_t size) noexcept;

        //______________________________________________________________________
        //
        //! crc32 of a type
        //______________________________________________________________________
        template <typename T> static inline
        uint32_t Of(const T &args) noexcept
        {
            return Of(&args,sizeof(T));
        }

    };

}

#endif
