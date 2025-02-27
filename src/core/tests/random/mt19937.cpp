#include "y/random/mt19937.hpp"
#include "y/utest/run.hpp"

#include "y/system/seed.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Random
    {
       

        const uint32_t MT19937:: ReferenceKey[4] = {
            0x123, 0x234, 0x345, 0x456
        };


        void MT19937:: init_genrand(const uint32_t s) noexcept
        {
            static const uint32_t fac = 1812433253UL;
            mt[0]= s & 0xffffffffUL;
            for (mti=1; mti<N; mti++) {
                mt[mti] =
                (fac * (mt[mti-1] ^ (mt[mti-1] >> 30)) + static_cast<uint32_t>(mti));
                /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
                /* In the previous versions, MSBs of the seed affect   */
                /* only MSBs of the array mt[].                        */
                /* 2002/01/09 modified by Makoto Matsumoto             */
                mt[mti] &= 0xffffffffUL;
                /* for >32 bit machines */
            }
        }

        /* initialize by an array with array-length */
        /* init_key is the array for initializing keys */
        /* key_length is its length */
        /* slight change for C++, 2004/2/26 */
        void MT19937:: init_by_array(const uint32_t init_key[],
                                     size_t         key_length) noexcept
        {
            size_t i, j, k;
            init_genrand(19650218UL);
            i=1; j=0;
            k = (N>key_length ? N : key_length);
            for (;k; k--) {
                mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
                + init_key[j] + static_cast<uint32_t>(j); /* non linear */
                mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
                i++; j++;
                if (i>=N) { mt[0] = mt[N-1]; i=1; }
                if (j>=key_length) j=0;
            }
            for (k=N-1; k; k--) {
                mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
                - static_cast<uint32_t>(i); /* non linear */
                mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
                i++;
                if (i>=N) { mt[0] = mt[N-1]; i=1; }
            }

            mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
        }


    }
}

using namespace Yttrium;

#include <cstdio>

Y_UTEST(random_mt19937)
{
    Random::MT19937 ran(Random::MT19937::ReferenceKey,Random::MT19937::ReferenceLen);
    printf("1000 outputs of genrand_int32()\n");
    for (size_t i=0; i<1000; i++) {
        printf("%10lu ", static_cast<unsigned long>(ran.next32()));
        if (i%5==4) printf("\n");
    }
}
Y_UDONE()

