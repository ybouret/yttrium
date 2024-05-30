
#include "y/kemp/element/pull64.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        uint64_t Pull64:: From(const uint8_t * const p) noexcept
        {
            assert(0!=p);
            const uint64_t w[8] = { p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7] };
            return w[0] | (w[1]<<8) | (w[2]<<16) | (w[3]<<24) | (w[4]<<32) | (w[5]<<40) | (w[6]<<48) | (w[7]<<56);
        }


        uint64_t Pull64:: From(const uint16_t * const p) noexcept
        {
            assert(0!=p);
            const uint64_t w[4] = { p[0], p[1], p[2], p[3] };
            return w[0] | (w[1]<<16) | (w[2]<<32) | (w[3]<<48);
        }

        uint64_t Pull64:: From(const uint32_t * const p) noexcept
        {
            assert(0!=p);
            const uint64_t w[2] = { p[0], p[1] };
            return w[0] | (w[1]<<32);
        }

        uint64_t Pull64:: From(const uint64_t * const p) noexcept
        {
            assert(0!=p);
            return *p;
        }

    }


}


