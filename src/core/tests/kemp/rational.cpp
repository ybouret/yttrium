
#include "y/kemp/rational.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;
using namespace Kemp;

Y_UTEST(kemp_rational)
{
    Random::ParkMiller ran;

    apq q; std::cerr << q << std::endl;

    q = apq(6,3);
    std::cerr << q << std::endl;

    q = apq(7,13); q *= apq(-4,5); std::cerr << q << std::endl;
    q = apq(7,13); q *= -4;        std::cerr << q << std::endl;
}
Y_UDONE()

