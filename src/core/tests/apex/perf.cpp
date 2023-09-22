
#include "y/apex/n/proto.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/utest/timing.hpp"

#include <cstdio>

using namespace Yttrium;


namespace
{

    static size_t Loops = 10;

    template <typename Core, typename Word> static inline
    void TestProto(Random::Bits &ran)
    {
        typedef Apex::Proto<Core,Word>  PROTO;
        typedef typename PROTO::Pointer hPROTO;

        static char label[256];
        snprintf(label,sizeof(label),"Apex::Proto<%3u,%3u>",PROTO::CoreSize*8,PROTO::WordSize*8 );
        std::cerr << std::endl;
        std::cerr << label << std::endl;

        const PROTO  lhs(ran.in<unsigned>(0,200),ran);
        const PROTO  rhs(ran.in<unsigned>(0,200),ran);
        const hPROTO LNG_Prod = PROTO::Mul(lhs,rhs,PROTO::LongMul,0);
        const hPROTO FFT_Prod = PROTO::Mul(lhs,rhs,PROTO::FFT_Mul,0);

        Y_CHECK(PROTO::AreEqual(*LNG_Prod, *FFT_Prod));

    }

}

Y_UTEST(apex_perf)
{

    Random::Rand ran;

    if(argc>1) Loops = unsigned( atol(argv[1]) );

    TestProto<uint64_t,uint32_t>( ran );
    TestProto<uint64_t,uint16_t>( ran );
    TestProto<uint64_t,uint8_t>(  ran );

    TestProto<uint32_t,uint16_t>( ran );
    TestProto<uint32_t,uint8_t>(  ran );

    TestProto<uint16_t,uint8_t>(  ran );

}
Y_UDONE()

