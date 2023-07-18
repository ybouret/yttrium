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

    static unsigned MaxBits = 512;
    static unsigned Loops   = 1;

    template <typename Core, typename Word> static inline
    void TestProto(Random::Bits &ran, Perf &perf)
    {
        typedef Apex::Proto<Core,Word>  PROTO;
        typedef typename PROTO::Pointer hPROTO;

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
            std::cerr << "\t"; proto.printHex(std::cerr); std::cerr << std::endl;
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
                    typename PROTO::Pointer  pS( PROTO::Add(ls.w,ls.n,rs.w,rs.n, &tmx.ticks) );
                    const uint64_t S = pS->ls64();
                    Y_ASSERT(S==s);
                    ++tmx.cycle;
                }
            }
        }
        std::cerr << HumanReadable(tmx.speed()) << "Op/s" << std::endl;


        (std::cerr << "   (*) Addition performance [").flush();
        tmx.reset();
        {
            for(unsigned i=8;i<=MaxBits;i += 8)
            {
                std::cerr << '.';
                for(unsigned j=8;j<=MaxBits; j += 8)
                {

                    for(size_t loop=0;loop<Loops;++loop)
                    {
                        PROTO lhs(i,ran); Y_ASSERT(lhs.nbits==i);
                        PROTO rhs(j,ran); Y_ASSERT(rhs.nbits==j);
                        const hPROTO sum( PROTO::Add(lhs,rhs, &tmx.ticks) );
                        ++tmx.cycle;
                    }
                }
            }
        }
        std::cerr << "]" << std::endl;
        std::cerr << "    @" << HumanReadable(perf.addRate=tmx.speed()) << "Op/s" << std::endl;

        (std::cerr << "   (*) Subtraction check64" << std::endl).flush();
        for(unsigned i=0;i<=63;++i)
        {
            for(unsigned j=0;j<=63;++j)
            {
                for(unsigned loop=0;loop<Loops;++loop)
                {
                    uint64_t l = ran.to<uint64_t>( i );
                    uint64_t r = ran.to<uint64_t>( j );
                    if(l<r) Swap(l,r);
                    const uint64_t d = l-r;
                    PROTO L( l );
                    PROTO R( r );
                    typename PROTO::Splitter ls(l);
                    typename PROTO::Splitter rs(r);
                    typename PROTO::Pointer  pD( PROTO::Sub(ls.w,ls.n,rs.w,rs.n) );
                    const uint64_t D  = pD->ls64();
                    Y_ASSERT(D==d);
                }
            }
        }
        (std::cerr << "   (*) Subtraction Testing  [").flush();

        for(unsigned i=8;i<=MaxBits;i += 8)
        {
            std::cerr << '.';
            for(unsigned j=8;j<=MaxBits; j += 8)
            {

                for(size_t loop=0;loop<Loops;++loop)
                {
                    const PROTO  lhs(i,ran);
                    const PROTO  rhs(j,ran);
                    const hPROTO sum( PROTO::Add(lhs,rhs) );
                    {
                        const hPROTO dif( PROTO::Sub(*sum,lhs) );
                        Y_ASSERT( PROTO::AreEqual(rhs,*dif)      );
                        Y_ASSERT( PROTO::AreEqual(*dif,rhs)      );
                        Y_ASSERT( !PROTO::AreDifferent(*dif,rhs) );
                        Y_ASSERT( !PROTO::AreDifferent(rhs,*dif) );
                    }

                    {
                        const hPROTO dif( PROTO::Sub(*sum,rhs) );
                        Y_ASSERT( PROTO::AreEqual(lhs,*dif)      );
                        Y_ASSERT( PROTO::AreEqual(*dif,lhs)      );
                        Y_ASSERT( !PROTO::AreDifferent(*dif,lhs) );
                        Y_ASSERT( !PROTO::AreDifferent(lhs,*dif) );
                    }
                }

            }
        }
        std::cerr << "]" << std::endl;


        (std::cerr << "   (*) Comparison 64-bits" << std::endl).flush();

        for(unsigned i=0;i<=64;++i)
        {
            for(unsigned j=0;j<=64;++j)
            {
                for(unsigned loop=0;loop<Loops;++loop)
                {
                    const uint64_t  l = ran.to<uint64_t>( i );
                    const uint64_t r = ran.to<uint64_t>( j );
                    const PROTO    lhs(l);
                    const PROTO    rhs(r);
                    Y_ASSERT(SignOf(l,r) == PROTO::Compare(lhs,rhs));
                }
            }
        }

        (std::cerr << "   (*) Comparison Checking" << std::endl).flush();
        for(unsigned i=64;i<=MaxBits;i += 2)
        {
            for(unsigned j=64;j<=MaxBits; j += 2)
            {
                if(i==j) continue;;

                for(size_t loop=0;loop<Loops;++loop)
                {
                    const PROTO  lhs(i,ran);
                    const PROTO  rhs(j,ran);
                    if(i<j)
                    {
                        Y_ASSERT( PROTO::Compare(lhs,rhs) == Negative ) ;
                    }
                    else
                    {
                        Y_ASSERT( PROTO::Compare(lhs,rhs) == Positive ) ;
                    }
                }

            }
        }

        std::cerr << "   (*) Powers Of Two" << std::endl;
        {
            for(unsigned i=0;i<64;++i)
            {
                const PROTO    p(i,Apex::AsShift);
                const uint64_t u=uint64_t(1)<<i;
                p.printHex(std::cerr << "2^" << i << " = "); std::cerr << "/" << Hexadecimal(u,Hexadecimal::Compact) << std::endl;
            }
        }

        {
            for(unsigned i=64;i<=257;++i)
            {
                const PROTO    p(i,Apex::AsShift);
                Y_ASSERT(p.nbits==i+1);
            }
        }

        std::cerr << "   (*) Multiplication64" << std::endl;
        for(unsigned i=0;i<=32;++i)
        {
            for(unsigned j=0;j<=32;++j)
            {
                const uint64_t l = ran.to<uint64_t>( i );
                const uint64_t r = ran.to<uint64_t>( j );
                const uint64_t p =l*r;
                const PROTO    lhs(l);
                const PROTO    rhs(r);
                const hPROTO   prod( PROTO::Mul(lhs,rhs,PROTO::LongMul,0) );
                std::cerr << "Prod = ";
                prod->printHex(std::cerr);  std::cerr << " / " << Hexadecimal(p,Hexadecimal::Compact,Hexadecimal::UpperCase) << std::endl;
                Y_ASSERT( prod->ls64() == p );
            }
        }

        std::cerr << "   (*) Mul power of two" << std::endl;
        for(unsigned i=0;i<=1000;i+=1+ran.leq(10))
        {
            const PROTO lhs(i,Apex::AsShift);
            for(unsigned j=0;j<=1000;j+=1+ran.leq(10))
            {
                const PROTO  rhs(j,Apex::AsShift);
                const hPROTO prod( PROTO::Mul(lhs,rhs,PROTO::LongMul,0) );
                const PROTO  theo(i+j,Apex::AsShift);
                Y_ASSERT( PROTO::AreEqual(*prod,theo) );
            }
        }


    }
}


Y_UTEST(apex_proto)
{
    Random::Rand ran;
    Perf         Table[6];

    if(argc>1) Loops = unsigned( atol(argv[1]) );

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
