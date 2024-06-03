

#include "y/kemp/natural.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;

Y_UTEST(kemp_muldiv)
{
    Random::ParkMiller ran;

    std::cerr << '[';
    for(size_t i=1;i<=512;i+=ran.in<size_t>(4,10) )
    {
        (std::cerr << '*').flush();
        for(size_t j=1;j<=512;j+= ran.in<size_t>(4,10))
        {

            const apn I(i,ran);
            const apn J(j,ran);
            {
                apn::MulByFFT = false;
                const apn P  = I*J;
                const apn D1 = P/I; Y_ASSERT(D1==J);
                const apn D2 = P/J; Y_ASSERT(D2==I);
            }
            {
                apn::MulByFFT = true;
                const apn P  = I*J;
                const apn D1 = P/I; Y_ASSERT(D1==J);
                const apn D2 = P/J; Y_ASSERT(D2==I);
            }
        }
    }
    std::cerr << ']' << std::endl;

}
Y_UDONE()
