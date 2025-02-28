
#include "y/random/mt19937.hpp"
#include "y/system/seed.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Random
    {
#define Y_MT19937_CTOR()   \
Random::Bits(0xffffffff),  \
mti(MTI),                  \
mt()
        MT19937:: MT19937()   :
        Y_MT19937_CTOR()
        {
            init_genrand( SystemSeed::Get() );
        }

        MT19937:: MT19937(const uint32_t s) noexcept :
        Y_MT19937_CTOR()
        {
            init_genrand(s);
        }


        MT19937:: MT19937(const uint32_t init_key[], const size_t key_length) noexcept :
        Random::Bits(0xffffffff),
        mti(MTI),
        mt()
        {
            init_by_array(init_key,key_length);
        }


        MT19937:: ~MT19937() noexcept
        {
            mti = MTI;
            memset(mt,0,sizeof(mt));
        }


        uint32_t MT19937:: next32(void) noexcept
        {
            static const uint32_t mag01[2]={0x0UL, MATRIX_A};

            /* mag01[x] = x * MATRIX_A  for x=0,1 */

            if (mti >= N) { /* generate N words at one time */
                size_t kk;

                if (mti == N+1)   /* if init_genrand() has not been called, */
                    init_genrand(5489UL); /* a default initial seed is used */

                for (kk=0;kk<N-M;kk++) {
                    const uint32_t y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
                    mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
                }
                for (;kk<N-1;kk++) {
                    const uint32_t y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
                    mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
                }

                {
                    const uint32_t y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
                    mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];
                }
                mti = 0;
            }

            uint32_t y = mt[mti++];

            /* Tempering */
            y ^= (y >> 11);
            y ^= (y << 7)  & 0x9d2c5680UL;
            y ^= (y << 15) & 0xefc60000UL;
            y ^= (y >> 18);

            return y;
        }

        const char * const MT19937:: CallSign = "MT19937";

        const char * MT19937:: callSign() const noexcept { return CallSign; }
    }

}


