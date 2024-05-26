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

    APK::Element el(0);
    for(unsigned i=0;i<=64;++i)
    {
        const uint64_t qw = ran.to<uint64_t>(i); Y_ASSERT(i==BitCount::For(qw));
        el.u64(qw);
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
        std::cerr << "<Internal Representations>" << std::endl;
        APK::Element el;
        el.show(std::cerr << "el=") << std::endl;
        el.u64( ran.to<uint64_t>());
        el.show(std::cerr << "el=") << std::endl;
        el.set(APK::AsBytes);
        el.show(std::cerr << "el=") << std::endl;
        el.set(APK::AsNum16);
        el.show(std::cerr << "el=") << std::endl;
        el.set(APK::AsNum32);
        el.show(std::cerr << "el=") << std::endl;
    }

    {
        std::cerr << "<Random Bits>" << std::endl;
        for(unsigned i=0;i<=80;++i)
        {
            APK::Element el(i,ran); Y_ASSERT(i==el.bits);
            el.show(std::cerr << "el8 =") << std::endl;
            el.set(APK::AsNum16);
            el.show(std::cerr << "el16=") << std::endl;
            el.set(APK::AsNum32);
            el.show(std::cerr << "el32=") << std::endl;
            el.set(APK::AsNum64);
            el.show(std::cerr << "el64=") << std::endl;
        }
    }

    {
        std::cerr << "<Read U64>" << std::endl;
        for(unsigned i=0;i<=80;++i)
        {
            APK::Element el(i,ran);
            Y_ASSERT(APK::AsBytes==el.state);
            Y_ASSERT(i==el.bits);
            const uint64_t q1 = el.u64();
            const uint64_t q2 = el.set(APK::AsNum16).u64();
            const uint64_t q4 = el.set(APK::AsNum32).u64();
            const uint64_t q8 = el.set(APK::AsNum64).u64();

            //std::cerr << "q1 = " << Hexadecimal(q1) << std::endl;
            //std::cerr << "q2 = " << Hexadecimal(q2) << std::endl;
            //std::cerr << "q4 = " << Hexadecimal(q4) << std::endl;
            //std::cerr << "q8 = " << Hexadecimal(q8) << std::endl;
            Y_ASSERT(q1==q2);
            Y_ASSERT(q1==q4);
            Y_ASSERT(q1==q8);
        }
    }

}
Y_UDONE()

