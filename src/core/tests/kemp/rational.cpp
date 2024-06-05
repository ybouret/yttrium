
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
    q = apq(7,13); q /= apq(5,4);   std::cerr << q << std::endl;

    q = apq(7,13); apz z = -3; q/=z; std::cerr << q << std::endl;

    q = apq(7,13); apn n =  7; q/=n; std::cerr << q << std::endl;
    q = apq(7,13);  q/=8;            std::cerr << q << std::endl;

    q = apq(7,13);
    z = -3;
    std::cerr << z << "/(" << q << ") = " << z/q << std::endl;

    n = 3;
    std::cerr << n << "/(" << q << ") = " << n/q << std::endl;


}
Y_UDONE()

