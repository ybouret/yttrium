
#include "y/apex/n/proto.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/utest/timing.hpp"
#include "y/stream/libc/output.hpp"
#include "y/string.hpp"

#include <cstdio>
#include <cstring>

using namespace Yttrium;


namespace
{

    static size_t   Loops   = 8;
    static unsigned MaxBits = 1024;

    template <typename Core, typename Word> static inline
    void TestProto(Random::Bits &ran)
    {
        typedef Apex::Proto<Core,Word>  PROTO;
        typedef typename PROTO::Pointer hPROTO;
        static char label[256];


        memset(label,0,sizeof(label));
        snprintf(label,sizeof(label),"Apex::Proto<%2u,%2u>",PROTO::CoreSize*8,PROTO::WordSize*8 );
        std::cerr << std::endl;
        std::cerr << label << std::endl;



        if(true)
        {
            std::cerr << "-- Checking for products equality" << std::endl;
            for(size_t i=0;i<Loops;++i)
            {
                const PROTO  lhs(5000,ran);
                const PROTO  rhs(5000,ran);
                const hPROTO LNG_Prod = PROTO::Mul(lhs,rhs,PROTO::LongMul,0);
                const hPROTO FFT_Prod = PROTO::Mul(lhs,rhs,PROTO::FFT_Mul,0);
                const PROTO &LongProduct = *LNG_Prod;
                const PROTO &FFT_Product = *FFT_Prod;
                Y_ASSERT( PROTO::AreEqual(LongProduct,FFT_Product) );
            }

            std::cerr << "-- Checking for squares  equality" << std::endl;
            for(size_t i=0;i<Loops;++i)
            {
                const PROTO  lhs(5000,ran);
                const hPROTO LNG_Prd = PROTO::Mul(lhs,lhs,PROTO::LongMul,0);
                const hPROTO LNG_Sqr = PROTO::Sqr(lhs,PROTO::LongSqr,0);
                const hPROTO FFT_Sqr = PROTO::Sqr(lhs,PROTO::FFT_Sqr,0);

                const PROTO &DireSquare = *LNG_Prd;
                const PROTO &LongSquare = *LNG_Sqr;
                const PROTO &FFT_Square = *FFT_Sqr;

                Y_ASSERT( PROTO::AreEqual(DireSquare,LongSquare) );
                Y_ASSERT( PROTO::AreEqual(DireSquare,FFT_Square) );

            }
        }



        {
            const String fileName = Formatted::Get("mul%u-%u.dat",PROTO::CoreSize*8,PROTO::WordSize*8 );
            OutputFile   fp(fileName);
            for(unsigned lbits=32;lbits<=MaxBits;lbits <<= 1)
            {
                for(unsigned rbits=32;rbits<=MaxBits;rbits <<= 1)
                {
                    (std::cerr << "mul " << std::setw(6) << lbits << " x " << std::setw(6) << rbits << " : ").flush();
                    uint64_t l64 = 0;
                    uint64_t f64 = 0;
                    for(size_t i=0;i<Loops;++i)
                    {
                        const PROTO  lhs(lbits,ran);
                        const PROTO  rhs(rbits,ran);
                        const hPROTO LNG_Prod    = PROTO::Mul(lhs,rhs,PROTO::LongMul,&l64);
                        const hPROTO FFT_Prod    = PROTO::Mul(lhs,rhs,PROTO::FFT_Mul,&f64);
                        const PROTO &LongProduct = *LNG_Prod;
                        const PROTO &FFT_Product = *FFT_Prod;
                        Y_ASSERT( PROTO::AreEqual(LongProduct,FFT_Product) );
                    }
                    const double lrate = double(Loops)/double(l64);
                    const double frate = double(Loops)/double(f64);
                    std::cerr << "long: " << std::setw(15) << lrate << " | fft: " << std::setw(15) << frate;
                    if(frate>=lrate)
                    {
                        std::cerr << " (*) bytes = " << (lbits/8) << ":" << (rbits/8);
                    }
                    std::cerr << std::endl;
                    fp("%u %u %.15g %.15g\n", lbits, rbits, log10(lrate), log10(frate) );
                }
            }
        }

        const String fileName = Formatted::Get("sqr%u-%u.dat",PROTO::CoreSize*8,PROTO::WordSize*8 );
        OutputFile   fp(fileName);
        for(unsigned lbits=32;lbits<=MaxBits;lbits <<= 1)
        {
            (std::cerr << "sqr " << std::setw(6) << lbits << " : ").flush();
            uint64_t l64 = 0;
            uint64_t f64 = 0;
            for(size_t i=0;i<Loops;++i)
            {
                const PROTO  lhs(lbits,ran);
                const hPROTO LNG_Prod    = PROTO::Sqr(lhs,PROTO::LongSqr,&l64);
                const hPROTO FFT_Prod    = PROTO::Sqr(lhs,PROTO::FFT_Sqr,&f64);
                const PROTO &LongSquared = *LNG_Prod;
                const PROTO &FFT_Squared = *FFT_Prod;
                Y_ASSERT( PROTO::AreEqual(LongSquared,FFT_Squared) );
            }
            const double lrate = double(Loops)/double(l64);
            const double frate = double(Loops)/double(f64);
            std::cerr << "long: " << std::setw(15) << lrate << " | fft: " << std::setw(15) << frate;
            if(frate>=lrate)
            {
                std::cerr << " (*) bytes = " << (lbits/8);
            }
            std::cerr << std::endl;
            fp("%u %.15g %.15g\n", lbits, log10(lrate), log10(frate) );
        }



    }

}

#include <cmath>
#include <cfloat>

Y_UTEST(apex_perf)
{

    Random::Rand ran;

    if(argc>1) Loops   = unsigned( atol(argv[1]) );
    if(argc>2) MaxBits = unsigned( atol(argv[2]) );


    TestProto<uint16_t,uint8_t>(  ran );

    TestProto<uint32_t,uint8_t>(  ran );
    TestProto<uint32_t,uint16_t>( ran );


    TestProto<uint64_t,uint8_t>(  ran );
    TestProto<uint64_t,uint16_t>( ran );
    TestProto<uint64_t,uint32_t>( ran );

}
Y_UDONE()

