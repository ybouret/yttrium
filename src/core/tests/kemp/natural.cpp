#include "y/kemp/natural.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/text/hexadecimal.hpp"

using namespace Yttrium;

Y_UTEST(kemp_natural)
{
    Random::ParkMiller ran;
    for(unsigned i=0;i<=64;++i)
    {
        const uint64_t lhs = ran.to<uint64_t>(i);
        const apn      L(lhs);
        std::cerr << Hexadecimal(lhs,Hexadecimal::Compact) << "/" << L << std::endl;
    }

    for(unsigned i=0;i<=80;++i)
    {
        const apn n(Kemp::TwoToThe,i);
        std::cerr << n << std::endl;
    }
}
Y_UDONE()
