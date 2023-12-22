#include "y/calculus/prime.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(calculus_prime)
{
    uint64_t p      = 0;
    size_t   count = 0;
    while(count<10)
    {
        p = Prime64::Next(p);
        std::cerr << p << std::endl;
        ++count;
        ++p;
    }
}
Y_UDONE()

