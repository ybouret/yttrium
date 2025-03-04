
//! \file

#ifndef Y_Kemp_Pull64_Included
#define Y_Kemp_Pull64_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        //! reworking 64-bits input into another uint64_t
        struct Pull64
        {
            static uint64_t From(const uint8_t  * const) noexcept; //!< rebuild from bytes
            static uint64_t From(const uint16_t * const) noexcept; //!< rebuild from words
            static uint64_t From(const uint32_t * const) noexcept; //!< rebuild from dwords
            static uint64_t From(const uint64_t * const) noexcept; //!< read

        };

    }

}

#endif

