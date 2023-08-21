
#include "y/information/entropy.hpp"
#include "y/memory/allocator/dyadic.hpp"

#include <cmath>

namespace Yttrium
{
    namespace Information
    {

        Entropy:: ~Entropy() noexcept
        {
            void *block = count;
            Memory::Dyadic::Location().releaseBlock(block,shift);
            count = 0;
        }

        Entropy:: Entropy() :
        shift( iLog2Of<uint64_t>::Value + iLog2<256>::Value ),
        count( static_cast<uint64_t *>(Memory::Dyadic::Instance().acquireBlock(shift)) )
        {
            
        }

        Entropy & Entropy:: operator<<(const uint8_t c) noexcept
        {
            ++count[c];
            return *this;
        }

        Entropy & Entropy:: operator<<(const Memory::ReadOnlyBuffer &buff) noexcept
        {
            const uint8_t *p = static_cast<const uint8_t*>(buff.ro_addr());
            for(size_t i=buff.measure();i>0;--i)
            {
                ++count[ *(p++) ];
            }
            return *this;
        }

        long double Entropy:: operator()(void) const noexcept
        {
            uint64_t N = 0;
            for(size_t i=0;i<256;++i)
            {
                N += count[i];
            }

            long double res = 0;

            for(size_t i=0;i<256;++i)
            {
                const uint64_t n = count[i];
                if(n<=0) continue;;
                const long double p = static_cast<long double>(n) / N;
                res += p * logl(p);
            }

            return -res;
        }

    }

}

