#include "y/apk/element.hpp"
#include "y/utest/run.hpp"
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
        el.ld(qw);
        Y_ASSERT(i==el.bits);
        std::cerr << Hexadecimal(qw)
        << " bits="  << std::setw(2) << el.bits
        << " bytes=" << std::setw(2) << el.bytes
        << " num16=" << std::setw(2) << el.num16
        << " num32=" << std::setw(2) << el.num32
        << " num64=" << std::setw(2) << el.num64
        << std::endl;

    }

    APK::Element::CheckTransmogrify();

    {
        APK::Element el;
        el.show(std::cerr << "el=") << std::endl;
        el.ld(0x123456);
        el.show(std::cerr << "el=") << std::endl;
        el.set(APK::AsBytes);
        el.show(std::cerr << "el=") << std::endl;
        el.set(APK::AsNum16);
        el.show(std::cerr << "el=") << std::endl;
        el.set(APK::AsNum32);
        el.show(std::cerr << "el=") << std::endl;
    }

    for(unsigned i=0;i<=80;++i)
    {
        std::cerr << "---- bits=" << i << std::endl;
        APK::Element el(i,ran);
        el.show(std::cerr << "el=") << std::endl;
        el.set(APK::AsNum16);
        el.show(std::cerr << "el=") << std::endl;
        el.set(APK::AsNum32);
        el.show(std::cerr << "el=") << std::endl;
        el.set(APK::AsNum64);
        el.show(std::cerr << "el=") << std::endl;
    }

    for(unsigned i=0;i<=80;++i)
    {
        std::cerr << "---- bits=" << i << std::endl;
        APK::Element el(i,ran);
        Y_ASSERT(APK::AsBytes==el.state);
        Y_ASSERT(i==el.bits);
        const uint64_t q1 = el.u64();
        const uint64_t q2 = el.set(APK::AsNum16).u64();
        const uint64_t q4 = el.set(APK::AsNum32).u64();
        const uint64_t q8 = el.set(APK::AsNum64).u64();

        std::cerr << Hexadecimal(q1) << "," << Hexadecimal(q2) << "," << Hexadecimal(q4) << "," << Hexadecimal(q8) << std::endl;

    }

}
Y_UDONE()

