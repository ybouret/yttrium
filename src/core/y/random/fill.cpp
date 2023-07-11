
#include "y/random/fill.hpp"

namespace Yttrium
{
    namespace Random
    {
        void Fill:: Block(void *blockAddr, size_t blockSize, Bits &ran) noexcept
        {
            assert( Good(blockAddr,blockSize) );
            uint8_t *p = static_cast<uint8_t *>(blockAddr);
            while(blockSize-- > 0)
            {
                *(p++) = ran.to<uint8_t>();
            }
        }

        static inline uint8_t GTZ(Bits &ran) noexcept
        {
        TRIAL:
            const uint8_t u = ran.to<uint8_t>();
            if(u>0) return u;
            goto TRIAL;
        }

        void Fill:: BlockGTZ(void *blockAddr, size_t blockSize, Bits &ran) noexcept
        {
            assert( Good(blockAddr,blockSize) );
            uint8_t *p = static_cast<uint8_t *>(blockAddr);
            while(blockSize-- > 0)
            {
                *(p++) = GTZ(ran);
            }
        }

    }

}

