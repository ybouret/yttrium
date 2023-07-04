
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

#if 0
        void     Hash32(uint32_t &lword, uint32_t &irword) noexcept
        {
            
            static uint32_t C1[4]={ 0xbaa96887L, 0x1e17d32cL, 0x03bcdc3cL, 0x0f33d1b2L };
            static uint32_t C2[4]={ 0x4b0f3b58L, 0xe874f0c3L, 0x6955c5a6L, 0x55a7ca46L };

            for (size_t i=0;i<4;i++)
            {
                const uint32_t iswap = irword;
                uint32_t       ia    = iswap ^ C1[i];
                const uint32_t itmpl = ia & 0xffff;
                const uint32_t itmph = ia >> 16;
                const uint32_t ib    = itmpl*itmpl+ ~(itmph*itmph);
                irword =lword ^ (((ia = (ib >> 16) | ((ib & 0xffff) << 16)) ^ C2[i])+itmpl*itmph);
                lword = iswap;
            }
        }

        uint32_t HRan32(uint32_t  seed[]) noexcept
        {
            uint32_t rword = seed[0]++;
            uint32_t lword = seed[1];
            Hash32(lword,rword);
            return rword;
        }
#endif

    }

}

