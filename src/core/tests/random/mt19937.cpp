#include "y/random/mt19937.hpp"
#include "y/utest/run.hpp"

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

