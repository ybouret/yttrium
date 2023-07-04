
#ifndef TestAleaIncluded
#define TestAleaIncluded 1

#include <cmath>
#include <ctime>
#include "y/check/crc32.hpp"
#include "y/memory/out-of-reach.hpp"

namespace
{

    inline unsigned alea_seed()
    {
        const time_t   tm = time(0);
        const uint32_t dw = Yttrium::CRC32::Of(tm);
        srand(dw);
        return dw;
    }

    inline double alea()
    {
        static volatile unsigned seed = alea_seed();
        (void)seed;
        return (0.5 + rand()) / (1.0+RAND_MAX);
    }

    inline size_t alea_leq(const size_t n)
    {
        return static_cast<size_t>(floor(double(n) * alea() + 0.5));
    }

    template <typename T> inline
    void alea_shuffle(T a[], const size_t n)
    {
        for(size_t i=n-1;i>0;--i)
        {
            const size_t j = alea_leq(i);
            Yttrium::Memory::OutOfReach::Swap(&a[i], &a[j], sizeof(T) );
        }
    }

}

#endif

