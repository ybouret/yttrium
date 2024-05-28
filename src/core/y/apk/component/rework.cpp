#include "y/apk/component/rework.hpp"

namespace Yttrium
{
    namespace APK
    {
        void Rework:: To(uint8_t  * const p, const uint64_t qw) noexcept
        {
            assert(0!=p);
            p[0] = static_cast<uint8_t>(qw);
            p[1] = static_cast<uint8_t>(qw>>8);
            p[2] = static_cast<uint8_t>(qw>>16);
            p[3] = static_cast<uint8_t>(qw>>24);
            p[4] = static_cast<uint8_t>(qw>>32);
            p[5] = static_cast<uint8_t>(qw>>40);
            p[6] = static_cast<uint8_t>(qw>>48);
            p[7] = static_cast<uint8_t>(qw>>56);
        }

        void Rework:: To(uint16_t  * const p, const uint64_t qw) noexcept
        {
            assert(0!=p);
            p[0] = static_cast<uint16_t>(qw);
            p[1] = static_cast<uint16_t>(qw>>16);
            p[2] = static_cast<uint16_t>(qw>>32);
            p[3] = static_cast<uint16_t>(qw>>48);

        }


    }

}

