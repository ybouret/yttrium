
#include "y/kemp/integer.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;

Y_UTEST(kemp_integer)
{
    Random::ParkMiller ran;


    for(int64_t i=-10;i<=10;++i)
    {
        const apz I = i;
        std::cerr << i << " => " << I << std::endl;
    }
}
Y_UDONE()
