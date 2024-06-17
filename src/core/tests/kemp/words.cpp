#include "y/kemp/element.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;
using namespace Kemp;

static inline
void DoSplit(Element &X, Element &Y, const Ops ops)
{
    Element::Words XP, YP;
    const size_t m = Element::Split(X, XP, Y, YP, ops);


    std::cerr << "m=" << m << std::endl;

    if(XP.lower.isValid() && XP.upper.isValid())
    {
        AutoPtr<Element> merged = Element::Merge(*XP.lower, *XP.upper, m, ops);
        std::cerr << "merged=" << merged << std::endl;
        Y_ASSERT( Element::Compare(*merged,X) == __Zero__ );
    }

    if(YP.lower.isValid() && YP.upper.isValid())
    {
        AutoPtr<Element> merged = Element::Merge(*YP.lower, *YP.upper, m, ops);
        std::cerr << "merged=" << merged << std::endl;
        Y_ASSERT( Element::Compare(*merged,Y) == __Zero__ );
    }
    std::cerr << std::endl;
}

Y_UTEST(kemp_words)
{
    Random::ParkMiller ran;

    for(size_t i=0;i<16;++i)
    {
        Element X( ran.in<size_t>(0,1000), ran );
        Element Y( ran.in<size_t>(0,1000), ran );

        std::cerr << X << std::endl;
        std::cerr << Y << std::endl;

        DoSplit(X,Y,Ops64_8);
        DoSplit(X,Y,Ops64_16);
        DoSplit(X,Y,Ops64_32);

    }


}
Y_UDONE()

