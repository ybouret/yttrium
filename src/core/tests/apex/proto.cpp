#include "y/apex/n/proto.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/utest/timing.hpp"

#include <cstdio>

using namespace Yttrium;


namespace
{



    static unsigned MaxBits = 1024;
    static unsigned Loops   = 4;

    template <typename Core, typename Word> static inline
    void TestProto(Random::Bits &ran)
    {
        typedef Apex::Proto<Core,Word>  PROTO;
        typedef typename PROTO::Pointer hPROTO;

        static char label[256];
        snprintf(label,sizeof(label),"Apex::Proto<%3u,%3u>",PROTO::CoreSize*8,PROTO::WordSize*8 );
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


        std::cerr << "   (*) Add64" << std::endl;
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
                    typename PROTO::Pointer  pS( PROTO::Add(ls.w,ls.n,rs.w,rs.n,0) );
                    const uint64_t S = pS->ls64();
                    Y_ASSERT(S==s);
                }
            }
        }



        (std::cerr << "   (*) Sub64" << std::endl).flush();
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


        std::cerr << "   (*) Add/Sub" << std::endl;
        for(unsigned i=8;i<=MaxBits;i += 8)
        {
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


        std::cerr << "   (*) Compare64" << std::endl;
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
                    Y_ASSERT(Sign::Of(l,r) == PROTO::Compare(lhs,rhs));
                }
            }
        }

        std::cerr << "   (*) Comparison" << std::endl;
        for(unsigned i=64;i<=MaxBits;i += 1+ran.leq(10))
        {
            for(unsigned j=64;j<=MaxBits; j += 1+ran.leq(10))
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
                Y_ASSERT(u==p.ls64());
            }
        }

        {
            for(unsigned i=64;i<=257;++i)
            {
                const PROTO    p(i,Apex::AsShift);
                Y_ASSERT(p.nbits==i+1);
            }
        }

        std::cerr << "   (*) Shift Right" << std::endl;
        {
            PROTO p(32,ran);
            while(p.nbits>0)
            {
                p.printHex(std::cerr); std::cerr << "|";
                p.shr();
            }
            std::cerr << std::endl;
        }


        std::cerr << "   (*) Mul64" << std::endl;
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
                Y_ASSERT( prod->ls64() == p );
            }
        }

        std::cerr << "   (*) Mul power of two" << std::endl;
        for(unsigned i=0;i<=500;i+=1+ran.leq(10))
        {
            const PROTO lhs(i,Apex::AsShift);
            for(unsigned j=0;j<=500;j+=1+ran.leq(10))
            {
                const PROTO  rhs(j,Apex::AsShift);
                const hPROTO prod( PROTO::Mul(lhs,rhs,PROTO::LongMul,0) );
                const PROTO  theo(i+j,Apex::AsShift);
                Y_ASSERT( PROTO::AreEqual(*prod,theo) );
            }
        }

        std::cerr << "   (*) FFT mul" << std::endl;
        {
            const PROTO lhs(30,ran);
            const PROTO rhs(30,ran);
            lhs.printHex(std::cerr << "\t0x"); std::cerr << std::endl;
            rhs.printHex(std::cerr << "\t0x"); std::cerr << std::endl;

            const hPROTO proto( PROTO::FFT_Mul(lhs.block.entry,lhs.words,
                                               rhs.block.entry,rhs.words,0) );
        }



    }
}


template <typename T>
static inline void testMakeBytes(Random::Bits &ran)
{
    T w = ran.to<T>();
    std::cerr << "-- Getting Bytes From " << Hexadecimal(w) << " =>";
    const uint8_t *u = Apex::MakeBytes::From(w);
    for(size_t i=0;i<sizeof(T);++i)
    {
        std::cerr << ' ' << Hexadecimal(u[i]);
    }
    std::cerr << std::endl;
}

Y_UTEST(apex_proto)
{
    Random::Rand ran;

    if(argc>1) Loops = unsigned( atol(argv[1]) );

    TestProto<uint64_t,uint32_t>( ran );
    return 0;
    
    TestProto<uint64_t,uint16_t>( ran );

    TestProto<uint64_t,uint8_t>(  ran );

    TestProto<uint32_t,uint16_t>( ran );
    TestProto<uint32_t,uint8_t>(  ran );

    TestProto<uint16_t,uint8_t>(  ran );

    testMakeBytes<uint8_t>(ran);
    testMakeBytes<uint16_t>(ran);
    testMakeBytes<uint32_t>(ran);


}
Y_UDONE()
