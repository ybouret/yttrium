#include "y/utest/run.hpp"
#include "y/apk/element.hpp"
#include "y/random/park-miller.hpp"
#include "y/calculus/bit-count.hpp"

#include <cstring>

using namespace Yttrium;


namespace Yttrium
{
    namespace APK
    {


      
    }
}

Y_UTEST(apk_n)
{
    Random::ParkMiller ran;

    for(size_t i=0;i<=20;++i)
    {

        APK::Element el(i);
        std::cerr << i << " => " << el.maxBytes << std::endl;

    }

    Y_SIZEOF(APK::Element);

    APK::Element el(0);
    for(unsigned i=0;i<=64;++i)
    {
        const uint64_t qw = ran.to<uint64_t>(i); Y_ASSERT(i==BitCount::For(qw));
        el.set(qw);
        Y_ASSERT(i==el.bits);
    }



}
Y_UDONE()

