
#include "y/apex/n/proto.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/utest/timing.hpp"
#include "y/stream/libc/output.hpp"

#include <cstdio>

using namespace Yttrium;


namespace
{

    static size_t Loops = 8;

    template <typename Core, typename Word> static inline
    void TestProto(Random::Bits &ran)
    {
        typedef Apex::Proto<Core,Word>  PROTO;
        typedef typename PROTO::Pointer hPROTO;

        static char label[256];
        snprintf(label,sizeof(label),"Apex::Proto<%3u,%3u>",PROTO::CoreSize*8,PROTO::WordSize*8 );
        std::cerr << std::endl;
        std::cerr << label << std::endl;

        Libc::OutputFile fp("apex-perf.dat");
        for(unsigned lbits=32;lbits<=1024;lbits+=32)
        {
            for(unsigned rbits=32;rbits<=1024;rbits+=32)
            {
                (std::cerr << std::setw(6) << lbits << " x " << std::setw(6) << rbits << " : ").flush();
                uint64_t l64 = 0;
                uint64_t f64 = 0;
                for(size_t i=0;i<Loops;++i)
                {
                    const PROTO  lhs(lbits,ran);
                    const PROTO  rhs(rbits,ran);
                    const hPROTO LNG_Prod = PROTO::Mul(lhs,rhs,PROTO::LongMul,&l64);
                    const hPROTO FFT_Prod = PROTO::Mul(lhs,rhs,PROTO::FFT_Mul,&f64);
                    const PROTO &LongProduct = *LNG_Prod;
                    const PROTO &FFT_Product = *FFT_Prod;
                    Y_ASSERT( PROTO::AreEqual(LongProduct,FFT_Product) );
                }
                const double lrate = double(l64)/Loops;
                const double frate = double(f64)/Loops;
                std::cerr << "long: " << HumanReadable(lrate) << " | fft: " << HumanReadable(frate) << std::endl;
                fp("%u %u %g %g\n", lbits, rbits, lrate, frate );
            }
        }

#if 0
        for(size_t i=0;i<Loops;++i)
        {
            const PROTO  lhs(ran.in<unsigned>(0,2000),ran);
            const PROTO  rhs(ran.in<unsigned>(0,2000),ran);
            const hPROTO LNG_Prod = PROTO::Mul(lhs,rhs,PROTO::LongMul,0);
            const hPROTO FFT_Prod = PROTO::Mul(lhs,rhs,PROTO::FFT_Mul,0);
            const PROTO &LongProduct = *LNG_Prod;
            const PROTO &FFT_Product = *FFT_Prod;

            Y_CHECK( PROTO::AreEqual(LongProduct,FFT_Product) );
        }
#endif
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

