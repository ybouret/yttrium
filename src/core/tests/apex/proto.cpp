

#include "y/apex/n/proto.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/utest/timing.hpp"

#include <cstdio>

using namespace Yttrium;


namespace
{

    struct Perf
    {
        const char *id;
        uint64_t    addRate;
    };

    static unsigned MaxBits = 1024;
    static unsigned Loops   = 16;

    template <typename Core, typename Word> static inline
    void TestProto(Random::Bits &ran, Perf &perf)
    {
        typedef Apex::Proto<Core,Word> PROTO;
        static char label[256];
        snprintf(label,sizeof(label),"Apex::Proto<%3u,%3u>",PROTO::CoreSize*8,PROTO::WordSize*8 );
        perf.id = label;
        std::cerr << std::endl;
        std::cerr << label << std::endl;
        
        // default
        std::cerr << "-- Checking Default" << std::endl;
        {
            PROTO proto(0,AsCapacity);
            const PROTO cpy(proto);
        }

        // test 64 bits
        std::cerr << "-- Checking 64 bits conversion" << std::endl;
        for(unsigned i=0;i<=64;++i)
        {
            for(size_t loop=0;loop<Loops;++loop)
            {
                const uint64_t u = ran.to<uint64_t>(i);
                PROTO proto(u);
                Y_ASSERT( proto.nbits == BitsFor(u) );
                //proto.display();
                const uint64_t p = proto.ls64();
                Y_ASSERT(p==u);
                const PROTO copy(proto);
            }
        }

        // test random bits
        std::cerr << "-- Checking Random Bits" << std::endl;
        for(unsigned i=0;i<=1000;++i)
        {
            for(size_t loop=0;loop<Loops;++loop)
            {
                PROTO proto(i,ran);
                Y_ASSERT(proto.nbits==i);
            }
        }

        std::cerr << "-- Checking Arbitrary Words conversion" << std::endl;
        {
            const Word W[] = { 1,2,3,4,5 };
            PROTO proto(W,sizeof(W)/sizeof(W[0]));
            std::cerr << "\t"; proto.display();
        }


        std::cerr << "-- Checking Operations" << std::endl;
        Timing tmx;


        (std::cerr << "   (*) Addition consistency @").flush();
        tmx.reset();
        for(unsigned i=0;i<=63;++i)
        {
            for(unsigned j=0;j<=63;++j)
            {
                for(unsigned loop=0;loop<Loops;++loop)
                {
                    const uint64_t l = ran.to<uint64_t>( i );
                    const uint64_t r = ran.to<uint64_t>( j );
                    const uint64_t s = l+r;
                    PROTO L( l );
                    PROTO R( r );
                    typename PROTO::Splitter ls(l);
                    typename PROTO::Splitter rs(r);
                    PROTO   *pS = PROTO::Add(ls.w,ls.n,rs.w,rs.n,tmx.ticks);
                    const uint64_t S = pS->ls64();
                    delete pS;
                    Y_ASSERT(S==s);
                    ++tmx.cycle;
                }
            }
        }
        std::cerr << HumanReadable(tmx.speed()) << "Op/s" << std::endl;


        (std::cerr << "   (*) Addition performance @").flush();
        tmx.reset();
        {
            for(unsigned i=8;i<=MaxBits;i += 8)
            {
                for(unsigned j=8;j<=MaxBits;++j)
                {
                    for(size_t loop=0;loop<Loops;++loop)
                    {
                        PROTO lhs(i,ran); Y_ASSERT(lhs.nbits==i);
                        PROTO rhs(j,ran); Y_ASSERT(rhs.nbits==j);
                        PROTO *sum = PROTO::Add(lhs,rhs,tmx.ticks);
                        ++tmx.cycle;
                        delete sum;
                    }
                }
            }
        }
        std::cerr << HumanReadable(perf.addRate=tmx.speed()) << "Op/s" << std::endl;


    }
}



Y_UTEST(apex_proto)
{
    Random::Rand ran;

    Perf Table[6];

    TestProto<uint64_t,uint32_t>( ran, Table[0]);
    TestProto<uint64_t,uint16_t>( ran, Table[1]);
    TestProto<uint64_t,uint8_t>(  ran, Table[2]);

    TestProto<uint32_t,uint16_t>( ran, Table[3]);
    TestProto<uint32_t,uint8_t>(  ran, Table[4]);

    TestProto<uint16_t,uint8_t>(  ran, Table[5]);

    std::cerr << std::endl;
    std::cerr << "Summary:" << std::endl;
    for(size_t i=0;i<6;++i)
    {
        const Perf &perf = Table[i];
        std::cerr << perf.id << ": add@" << HumanReadable(perf.addRate) << std::endl;
    }


}
Y_UDONE()
