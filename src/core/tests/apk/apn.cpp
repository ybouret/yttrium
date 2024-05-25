#include "y/utest/run.hpp"
#include "y/apk/element.hpp"
#include "y/random/park-miller.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/text/hexadecimal.hpp"

#include <cstring>

using namespace Yttrium;


namespace Yttrium
{
    namespace APK
    {


      
    }
}

#include "y/concurrent/mutex.hpp"

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
        std::cerr << Hexadecimal(qw)
        << " bits="  << std::setw(2) << el.bits
        << " bytes=" << std::setw(2) << el.bytes
        << " num16=" << std::setw(2) << el.num16
        << " num32=" << std::setw(2) << el.num32
        << " num64=" << std::setw(2) << el.num64

        << std::endl;

    }

    APK::Element::Check();

    Y_SIZEOF(Concurrent::Mutex);

}
Y_UDONE()

