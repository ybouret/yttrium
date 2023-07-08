
#include "y/type/hash32.hpp"

namespace Yttrium
{
    namespace Core
    {
        uint32_t Hash32(uint32_t a) noexcept
        {
            a = uint32_t(a+0x7ed55d16) + uint32_t(a<<12);
            a = uint32_t(a^0xc761c23c) ^ uint32_t(a>>19);
            a = uint32_t(a+0x165667b1) + uint32_t(a<<5);
            a = uint32_t(a+0xd3a2646c) ^ uint32_t(a<<9);
            a = uint32_t(a+0xfd7046c5) + uint32_t(a<<3);
            a = uint32_t(a^0xb55a4f09) ^ uint32_t(a>>16);
            return a;
        }


    }

}

