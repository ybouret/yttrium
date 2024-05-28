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


    std::cerr << "<uint64_t>" << std::endl;
    for(unsigned i=0;i<=64;++i)
    {
        const uint64_t qw = ran.to<uint64_t>(i);
        APK::Component cm( APK::FromNum64, qw);
        Y_ASSERT(cm.bits==i);
        std::cerr << Hexadecimal(qw) << "/" << cm << std::endl;
        Y_ASSERT( cm.set(APK::AsBytes).u64() == qw );
        Y_ASSERT( cm.set(APK::AsNum16).u64() == qw );
        Y_ASSERT( cm.set(APK::AsNum32).u64() == qw );
        Y_ASSERT( cm.set(APK::AsNum64).u64() == qw );
    }



    std::cerr << "<Compare64>" << std::endl;
    for(unsigned i=0;i<=64;++i)
    {
        const uint64_t l = ran.to<uint64_t>(i);
        APK::Component L( APK::FromNum64, l);
        Y_ASSERT(L.bits==i);

        for(unsigned k=0;k<4;++k)
        {
            L.set( APK::Component::State[k] );
            Y_ASSERT(__Zero__== APK::Component::Compare(L,L) );
        }

        for(unsigned j=0;j<=64;++j)
        {
            const uint64_t r = ran.to<uint64_t>(j);
            APK::Component R( APK::FromNum64, r);
            Y_ASSERT(R.bits==j);

            const SignType cmp = Sign::Of(l,r);

            for(unsigned p=0;p<4;++p)
            {
                for(unsigned q=0;q<4;++q)
                {
                    L.set( APK::Component::State[p] );
                    R.set( APK::Component::State[q] );
                    Y_ASSERT( APK::Component::Compare(L,R) == cmp );
                }
            }



        }
    }


    Y_SIZEOF(APK::Component);
}
Y_UDONE()

