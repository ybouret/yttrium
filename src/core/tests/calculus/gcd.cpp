
#include "y/calculus/gcd.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;

Y_UTEST(calculus_gcd)
{
    Y_ASSERT(GreatestCommonDivisor<unsigned>(15,5) == 5);
}
Y_UDONE()
