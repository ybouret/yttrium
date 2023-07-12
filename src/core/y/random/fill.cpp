
#include "y/random/fill.hpp"

namespace Yttrium
{
    namespace Random
    {
        void Fill:: Block(void  *       blockAddr,
                          size_t        blockSize,
                          Bits &        ran,
                          const uint8_t lo,
                          const uint8_t hi) noexcept
        {
            assert( Good(blockAddr,blockSize) );
            static const double half  = 0.5;
            const double        start = lo;
            const double        delta = double(hi) - double(lo);

            uint8_t *p = static_cast<uint8_t *>(blockAddr);
            while(blockSize-- > 0)
            {
                *(p++) = floor( start + ran.to<double>() * delta + half );
            }
        }

        



    }

}

