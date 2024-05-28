#include "y/apk/component/rework.hpp"

namespace Yttrium
{
    namespace APK
    {
        size_t Rework:: To(uint8_t  * const p, const uint64_t qw) noexcept
        {
            assert(0!=p);
            size_t count = 0;
            if( (p[0] = static_cast<uint8_t>(qw)    )>0 ) count=1;
            if( (p[1] = static_cast<uint8_t>(qw>>8) )>0 ) count=2;
            if( (p[2] = static_cast<uint8_t>(qw>>16))>0 ) count=3;
            if( (p[3] = static_cast<uint8_t>(qw>>24))>0 ) count=4;
            if( (p[4] = static_cast<uint8_t>(qw>>32))>0 ) count=5;
            if( (p[5] = static_cast<uint8_t>(qw>>40))>0 ) count=6;
            if( (p[6] = static_cast<uint8_t>(qw>>48))>0 ) count=7;
            if( (p[7] = static_cast<uint8_t>(qw>>56))>0 ) count=8;
            return count;
        }

        size_t Rework:: To(uint16_t  *const p, const uint64_t qw) noexcept
        {
            assert(0!=p);
            size_t count = 0;
            if( (p[0] = static_cast<uint16_t>(qw)     ) >0 ) count=1;
            if( (p[1] = static_cast<uint16_t>(qw>>16) ) >0 ) count=2;
            if( (p[2] = static_cast<uint16_t>(qw>>32) ) >0 ) count=3;
            if( (p[3] = static_cast<uint16_t>(qw>>48) ) >0 ) count=4;
            return count;
        }

        size_t Rework:: To(uint32_t  * const p,  const uint64_t qw) noexcept
        {
            assert(0!=p);
            size_t count = 0;
            if( (p[0] = static_cast<uint32_t>(qw)     ) >0 ) count=1;
            if( (p[1] = static_cast<uint32_t>(qw>>32) ) >0 ) count=2;
            return count;
        }

        size_t Rework:: To(uint64_t  * const,  const uint64_t qw) noexcept
        {
            return qw > 0 ? 1 : 0;
        }




    }

}

