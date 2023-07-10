
//! \file

#ifndef Y_SFH32_Included
#define Y_SFH32_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Super Fast Hash
    //
    //
    //__________________________________________________________________________
    struct SFH32
    {
        //______________________________________________________________________
        //
        //! updated current crc from addr[size]
        //______________________________________________________________________
        static uint32_t Run(uint32_t    hash,
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
