
#include "y/kemp/natural.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/calculus/bit-count.hpp"

using namespace Yttrium;

Y_UTEST(kemp_addsub)
{
    Random::ParkMiller ran;

    for(size_t i=0;i<=4096;i+=ran.in<size_t>(4,10) )
    {
        for(size_t j=0;j<=4096;j+= ran.in<size_t>(4,10))
        {
            const apn I(i,ran);
            const apn J(j,ran);
            const apn S = I+J;
            const apn D1 = S-I; Y_ASSERT(D1==J);
            const apn D2 = S-J; Y_ASSERT(D2==I);
        }
    }

}
Y_UDONE()
