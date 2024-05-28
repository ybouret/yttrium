
//! \file

#ifndef Y_APK_Component_Pull64_Included
#define Y_APK_Component_Pull64_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace APK
    {

        //! reworking 64-bits input into another uint64_t
        struct Pull64
        {
            static uint64_t From(const uint8_t * const) noexcept;
            static uint64_t From(const uint16_t * const) noexcept;
            static uint64_t From(const uint32_t * const) noexcept;
            static uint64_t From(const uint64_t * const) noexcept;

        };

    }

}

#endif

