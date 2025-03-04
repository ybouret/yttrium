//! \file

#ifndef Y_Kemp_Push64_Included
#define Y_Kemp_Push64_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        //! reworking 64-bits input into another uint64_t
        struct Push64
        {
            static size_t To(uint8_t  * const p, const uint64_t qw) noexcept; //!< split, return bytes
            static size_t To(uint16_t * const p, const uint64_t qw) noexcept; //!< split, return words
            static size_t To(uint32_t * const p, const uint64_t qw) noexcept; //!< split, return dwords
            static size_t To(uint64_t * const p, const uint64_t qw) noexcept; //!< do nothing, return qw>0
        };

    }

}

#endif

