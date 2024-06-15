#include "y/kemp/element.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;
using namespace Kemp;

Y_UTEST(kemp_words)
{
    Random::ParkMiller ran;

    {
        Element X( ran.in<size_t>(0,100), ran );
        Element Y( ran.in<size_t>(0,100), ran );

        std::cerr << X << std::endl;
        std::cerr << Y << std::endl;

        Element::Pair XP, YP;
        {
            const size_t m = Element::Split(X, XP, Y, YP, Ops64_8);
            std::cerr << "m8=" << m << std::endl;
        }

        {
            const size_t m = Element::Split(X, XP, Y, YP, Ops64_16);
            std::cerr << "m16=" << m << std::endl;
        }

        {
            const size_t m = Element::Split(X, XP, Y, YP, Ops64_32);
            std::cerr << "m32=" << m << std::endl;
        }


    }

#if 0
    for(size_t i=0;i<=19;++i)
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
            {
                AutoPtr<Element> res = Element::Merge(Ops64_8, *lo, *up, m);
                Y_ASSERT( __Zero__ == Element::Compare(*res,el) );
            }

            {
                AutoPtr<Element> res = Element::Merge(Ops32_8, *lo, *up, m);
                Y_ASSERT( __Zero__ == Element::Compare(*res,el) );
            }

            {
                AutoPtr<Element> res = Element::Merge(Ops16_8, *lo, *up, m);
                Y_ASSERT( __Zero__ == Element::Compare(*res,el) );
            }


        }

        std::cerr << el.set(AsNum16) << std::endl;
        {
            const size_t m = el.split(lo,up);
            std::cerr << "lo = " << lo << std::endl;
            std::cerr << "up = " << up << std::endl;
            std::cerr << "m  = " << m  << std::endl;

            {
                AutoPtr<Element> res = Element::Merge(Ops64_16, *lo, *up, m);
                Y_ASSERT( __Zero__ == Element::Compare(*res,el) );
            }

            {
                AutoPtr<Element> res = Element::Merge(Ops32_16, *lo, *up, m);
                Y_ASSERT( __Zero__ == Element::Compare(*res,el) );
            }

        }

        std::cerr << el.set(AsNum32) << std::endl;
        {
            const size_t m = el.split(lo,up);
            std::cerr << "lo = " << lo << std::endl;
            std::cerr << "up = " << up << std::endl;
            std::cerr << "m  = " << m  << std::endl;
            {
                AutoPtr<Element> res = Element::Merge(Ops64_32, *lo, *up, m);
                Y_ASSERT( __Zero__ == Element::Compare(*res,el) );
            }
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
#endif

}
Y_UDONE()

