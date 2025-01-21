
#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;
using namespace Apex;

Y_UTEST(apex_n)
{
    Random::ParkMiller ran;
    Natural n = 1025;

    std::cerr << n << std::endl;
    n = 111;
    std::cerr << n << std::endl;


    Y_SIZEOF(Natural);

    const size_t maxBits=1024;

    for(size_t lbits=0;lbits<=maxBits;++lbits)
    {
        for(size_t rbits=lbits;rbits<=maxBits;++rbits)
        {
            Natural lhs(ran,lbits);

        }
    }


}
Y_UDONE()
