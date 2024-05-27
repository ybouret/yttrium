#include "y/tow/api.hpp"
#include "y/apk/component.hpp"

#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/type/utils.hpp"
#include "y/system/exception.hpp"
#include <cstring>

namespace Yttrium
{
    namespace APK
    {



    }

}

#include "y/sequence/vector.hpp"

using namespace Yttrium;

Y_UTEST(apk_component)
{

    Random::ParkMiller ran;

    for(size_t i=0;i<=80;++i)
    {
        APK::Component cm(i);
        Y_ASSERT(cm.bytes.space>=i);
        //std::cerr << cm << std::endl;
    }

    for(size_t i=0;i<=64;++i)
    {
        APK::Component cm(i,ran); Y_ASSERT(APK::AsBytes==cm.state);
        std::cerr << cm << std::endl;
        std::cerr << cm.set(APK::AsNum16) << std::endl;
        std::cerr << cm.set(APK::AsNum32) << std::endl;
        std::cerr << cm.set(APK::AsNum64) << std::endl;
        std::cerr << std::endl;
    }


    for(size_t i=1;i<=1024;++i)
    {
        APK::Component  cm(i,ran);
        Vector<uint8_t> org(cm.bytes.count,AsCapacity);
        for(size_t j=0;j<cm.bytes.count;++j) org << cm.bytes.entry[j];
        Y_ASSERT(0==memcmp(cm.bytes.entry,&org[1],cm.bytes.count));

        for(size_t j=0;j<10;++j)
        {
            cm.set( APK::Component::State[ ran.in<unsigned>(0,3)] );
        }
        cm.set(APK::AsBytes);
        Y_ASSERT(0==memcmp(cm.bytes.entry,&org[1],cm.bytes.count));


    }


    Y_SIZEOF(APK::Component);
}
Y_UDONE()

