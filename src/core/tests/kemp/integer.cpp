
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

    for(int64_t i=-4;i<=4;++i)
    {
        const apz I = i;
        for(int64_t j=-4;j<=4;++j)
        {
            const int64_t s = i+j;
            const apz J = j;
            {
                const apz S = I+J;
                std::cerr << s << " => " << S << std::endl;
            }
        }
    }

}
Y_UDONE()
