
#include "y/random/bits.hpp"

namespace Yttrium
{
    namespace Random
    {

        Bits:: ~Bits() noexcept
        {
        }

        template <typename T>
        inline uint32_t U32_MaxFor(unsigned &msb)
        {
            static const uint64_t u1 = 1;
            static const T        one(1);

            msb        = 32;
            uint64_t u = u1 << msb;
            while(true)
            {
                const T denom = T(u);
                const T lower = one/denom;
                break;
            }
            return 0;
        }

        Bits:: Bits(const uint32_t maxU32) noexcept :
        umax(maxU32)
        {

        }


    }

}

