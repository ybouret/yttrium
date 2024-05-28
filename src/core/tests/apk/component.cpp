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

    std::cerr << "<Rework>" << std::endl;
    for(unsigned i=0;i<=64;++i)
    {

        const uint64_t qw = ran.to<uint64_t>(i);
        std::cerr << Hexadecimal(qw) << std::endl;
        {
            uint64_t                     work = qw;
            const APK::Assembly<uint8_t> llqw(work);
            std::cerr << "\t" << llqw << std::endl;
        }

        {
            uint64_t                     work = qw;
            const APK::Assembly<uint16_t> llqw(work);
            std::cerr << "\t" << llqw << std::endl;
        }


        {
            uint64_t                      work = qw;
            const APK::Assembly<uint32_t> llqw(work);
            std::cerr << "\t" << llqw << std::endl;
        }

        {
            uint64_t                      work = qw;
            const APK::Assembly<uint64_t> llqw(work);
            std::cerr << "\t" << llqw << std::endl;
        }

    }

    
    std::cerr << "<Empty>" << std::endl;
    for(size_t i=0;i<=80;++i)
    {
        APK::Component cm(i);
        Y_ASSERT(cm.bytes.space>=i);
    }


    std::cerr << "<Random>" << std::endl;
    for(size_t i=0;i<=64;++i)
    {
        APK::Component cm(i,ran); Y_ASSERT(APK::AsBytes==cm.state);
        std::cerr << cm << std::endl;
        std::cerr << cm.set(APK::AsNum16) << std::endl;
        std::cerr << cm.set(APK::AsNum32) << std::endl;
        std::cerr << cm.set(APK::AsNum64) << std::endl;
        std::cerr << std::endl;
    }

    std::cerr << "<Scramble>" << std::endl;
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


#if 0
    std::cerr << "<uint64_t>" << std::endl;
    for(unsigned i=0;i<=64;++i)
    {
        const uint64_t qw = ran.to<uint64_t>(i);
        APK::Component cm( APK::FromNum64, qw);
        Y_ASSERT(cm.bits==i);
        std::cerr << Hexadecimal(qw) << "/" << cm << std::endl;
        {
            uint64_t     temp = qw;
            size_t       size = 0;
            void * const addr = APK::Component::Tuned(APK::AsNum64, temp, size);
            cm.set(APK::AsNum64);
            Y_ASSERT(size==cm.num64.count);
            Y_ASSERT(0==memcmp(addr,cm.num64.entry,size*sizeof(uint64_t)));
        }

        {
            uint64_t     temp = qw;
            size_t       size = 0;
            void * const addr = APK::Component::Tuned(APK::AsNum32, temp, size);
            cm.set(APK::AsNum32);
            Y_ASSERT(size==cm.num32.count);
            Y_ASSERT(0==memcmp(addr,cm.num32.entry,size*sizeof(uint32_t)));
        }

        {
            uint64_t     temp = qw;
            size_t       size = 0;
            void * const addr = APK::Component::Tuned(APK::AsNum16, temp, size);
            cm.set(APK::AsNum16);
            Y_ASSERT(size==cm.num16.count);
            Y_ASSERT(0==memcmp(addr,cm.num16.entry,size*sizeof(uint16_t)));
        }

        {
            uint64_t     temp = qw;
            size_t       size = 0;
            void * const addr = APK::Component::Tuned(APK::AsBytes, temp, size);
            cm.set(APK::AsNum32);
            Y_ASSERT(size==cm.bytes.count);
            Y_ASSERT(0==memcmp(addr,cm.bytes.entry,size*sizeof(uint8_t)));
        }

        Y_ASSERT( cm.set(APK::AsBytes).u64() == qw );
        Y_ASSERT( cm.set(APK::AsNum16).u64() == qw );
        Y_ASSERT( cm.set(APK::AsNum32).u64() == qw );
        Y_ASSERT( cm.set(APK::AsNum64).u64() == qw );

    }
#endif




    Y_SIZEOF(APK::Component);
}
Y_UDONE()

