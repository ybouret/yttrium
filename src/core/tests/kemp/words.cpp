#include "y/kemp/element.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;
using namespace Kemp;

Y_UTEST(kemp_words)
{
    Random::ParkMiller ran;

    for(size_t i=0;i<=17;++i)
    {
        Element          el(i*8,ran);
        AutoPtr<Element> lo;
        AutoPtr<Element> up;

        std::cerr << el.set(AsBytes) << std::endl;
        {
            const size_t m = el.split(lo,up);
            std::cerr << "lo = " << lo << std::endl;
            std::cerr << "up = " << up << std::endl;
            std::cerr << "m  = " << m  << std::endl;
        }

        std::cerr << el.set(AsNum16) << std::endl;
        {
            const size_t m = el.split(lo,up);
            std::cerr << "lo = " << lo << std::endl;
            std::cerr << "up = " << up << std::endl;
            std::cerr << "m  = " << m  << std::endl;
        }

        std::cerr << el.set(AsNum32) << std::endl;
        {
            const size_t m = el.split(lo,up);
            std::cerr << "lo = " << lo << std::endl;
            std::cerr << "up = " << up << std::endl;
            std::cerr << "m  = " << m  << std::endl;
        }

        std::cerr << el.set(AsNum64) << std::endl;
        {
            const size_t m = el.split(lo,up);
            std::cerr << "lo = " << lo << std::endl;
            std::cerr << "up = " << up << std::endl;
            std::cerr << "m  = " << m  << std::endl;
        }


        std::cerr << std::endl;
    }

}
Y_UDONE()

