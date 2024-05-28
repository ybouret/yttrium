//! \file

#ifndef Y_APK_Component_Rework_Included
#define Y_APK_Component_Rework_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace APK
    {

        struct Rework
        {
            static void To(uint8_t  * const p, const uint64_t qw) noexcept;
            static void To(uint16_t * const p, const uint64_t qw) noexcept;
            static void To(uint32_t * const p, const uint64_t qw) noexcept;
            static void To(uint64_t * const p, const uint64_t qw) noexcept;
        };

    }

}

#endif

