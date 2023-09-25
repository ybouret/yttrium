#include "y/calculus/bit-count.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/random/bits.hpp"
#include <cstdio>

using namespace Yttrium;

template <typename T>
static inline void showBase2(const char *name)
{
    std::cerr << "Base2<" << name << ">" << std::endl;
    std::cerr << "\tMaxShift=" << Base2<T>::MaxShift      << std::endl;
    std::cerr << "\tMaxSlots=" << Base2<T>::MaxSlots      << std::endl;
    std::cerr << "\tMaxPower=" << Base2<T>::MaxPowerOfTwo << std::endl;

}


#define SHOW(CLASS) showBase2<CLASS>(#CLASS)

static inline
void Call8_()
{
    volatile unsigned toto = 0;
    for(unsigned i=0;i<256;++i)
    { const uint8_t b(i); toto = BitsFor(b); }
    (void)Memory::OutOfReach::Addr((void*)&toto);
}

static inline
void Call8()
{
    volatile unsigned toto = 0;
    for(unsigned i=0;i<256;++i) { const uint8_t b(i); toto = BitCount:: For(b);  }
    (void)Memory::OutOfReach::Addr((void*)&toto);
}


static inline
void Call16_()
{
    volatile unsigned toto = 0;
    for(unsigned i=0;i<65536;++i) { const uint16_t b(i); toto = BitsFor(b); }
    (void)Memory::OutOfReach::Addr((void*)&toto);
}

static inline
void Call16()
{
    volatile unsigned toto = 0;
    for(unsigned i=0;i<65536;++i) { const uint16_t b(i); toto = BitCount:: For(b);   }
    (void)Memory::OutOfReach::Addr((void*)&toto);
}

static inline
void Call32_(const uint32_t *u, unsigned n)
{
    volatile unsigned toto = 0;
    for(unsigned i=0;i<n;++i) {  toto = BitsFor(u[i]);   }
    (void)Memory::OutOfReach::Addr((void*)&toto);
}

static inline
void Call32(const uint32_t *u, unsigned n)
{
    volatile unsigned toto = 0;
    for(unsigned i=0;i<n;++i) {  toto = BitCount:: For(u[i]);   }
    (void)Memory::OutOfReach::Addr((void*)&toto);
}

static inline
void Call64_(const uint64_t *u, unsigned n)
{
    volatile unsigned toto = 0;
    for(unsigned i=0;i<n;++i) {  toto = BitsFor(u[i]);   }
    (void)Memory::OutOfReach::Addr((void*)&toto);
}


static inline
void Call64(const uint64_t *u, unsigned n)
{
    volatile unsigned toto = 0;
    for(unsigned i=0;i<n;++i) {  toto = BitCount:: For(u[i]);   }
    (void)Memory::OutOfReach::Addr((void*)&toto);
}

#include "y/text/hexadecimal.hpp"

Y_UTEST(calculus_base2)
{

    Random::Rand ran;
    SHOW(int32_t);
    SHOW(uint64_t);


    std::cerr << "Check Random Bits" << std::endl;
    {
        for(unsigned nbit=0;nbit<=64;++nbit)
        {
            std::cerr << Hexadecimal(  ran.to<uint64_t>(nbit), Hexadecimal::Compact) << std::endl;
            for(size_t loop=0;loop<1000;++loop)
            {
                const uint64_t qw = ran.to<uint64_t>(nbit);
                Y_ASSERT(nbit==BitsFor(qw));
            }
        }

    }

    {
        unsigned nbit = 0;
        for(uint32_t i=0;i<65536;++i)
        {
            const unsigned n = BitsFor(i);
            if(n>nbit)
            {
                std::cerr << i << " -> " << n << std::endl;
                nbit = n;
            }
        }
    }

#if 0
    {
        unsigned k = 0;
        for(unsigned i=0;i<16;++i)
        {
            for(unsigned j=0;j<16;++j,++k)
            {
                fprintf(stderr," 0x%1x, ", BitsFor_(k));
            }
            fprintf(stderr,"\n");
        }
    }
#endif


    std::cerr << "Checking Bit Count" << std::endl;
    std::cerr << "\tCheck Table" << std::endl;
    for(unsigned i=0;i<256;++i)
    {
        Y_ASSERT(BitsFor(i) == BitCount::Table[i] );
    }

    std::cerr << "\tCheck 8-bits" << std::endl;
    for(unsigned i=0;i<256;++i)
    {
        const uint8_t b(i);
        Y_ASSERT(BitsFor(i) == BitCount::For(b) );
    }

    Timing tmx;
    Y_Timing(tmx,Call8_(),1);
    std::cerr << "\t\traw: " << HumanReadable(tmx.speed()) << std::endl;
    Y_Timing(tmx,Call8(),1);
    std::cerr << "\t\topt: " << HumanReadable(tmx.speed()) << std::endl;


    std::cerr << "\tCheck 16-bits" << std::endl;
    for(unsigned i=0;i<65536;++i)
    {
        const uint16_t w(i);
        Y_ASSERT(BitsFor(i) == BitCount::For(w) );
    }
    Y_Timing(tmx,Call16_(),1);
    std::cerr << "\t\traw: " << HumanReadable(tmx.speed()) << std::endl;
    Y_Timing(tmx,Call16(),1);
    std::cerr << "\t\topt: " << HumanReadable(tmx.speed()) << std::endl;


    std::cerr << "\tCheck 32-bits" << std::endl;
    for(unsigned nbit=0;nbit<=32;++nbit)
    {
        for(size_t loop=0; loop<10000; ++loop)
        {
            const uint32_t dw = ran.to<uint32_t>(nbit);
            Y_ASSERT(nbit==BitsFor(dw));
            Y_ASSERT(nbit==BitCount::For(dw));
        }
    }

    {
        uint32_t   u32[4096];
        for(size_t i=0; i < sizeof(u32)/sizeof(u32[0]); ++i)
        {
            u32[i] = ran.to<uint32_t>( ran.in<unsigned>(1,32) );
        }
        Y_Timing(tmx,Call32_(u32,sizeof(u32)/sizeof(u32[0])),1);
        std::cerr << "\t\traw: " << HumanReadable(tmx.speed()) << std::endl;
        Y_Timing(tmx,Call32(u32,sizeof(u32)/sizeof(u32[0])),1);
        std::cerr << "\t\topt: " << HumanReadable(tmx.speed()) << std::endl;
    }

    std::cerr << "\tCheck 64-bits" << std::endl;
    for(unsigned nbit=0;nbit<=64;++nbit)
    {
        for(size_t loop=0; loop<10000; ++loop)
        {
            const uint64_t qw = ran.to<uint64_t>(nbit);
            Y_ASSERT(nbit==BitsFor(qw));
            Y_ASSERT(nbit==BitCount::For(qw));
        }
    }

    {
        uint64_t   u64[4096];
        for(size_t i=0; i < sizeof(u64)/sizeof(u64[0]); ++i)
        {
            u64[i] = ran.to<uint64_t>( ran.in<unsigned>(1,64) );
        }
        Y_Timing(tmx,Call64_(u64,sizeof(u64)/sizeof(u64[0])),1);
        std::cerr << "\t\traw: " << HumanReadable(tmx.speed()) << std::endl;
        Y_Timing(tmx,Call64(u64,sizeof(u64)/sizeof(u64[0])),1);
        std::cerr << "\t\topt: " << HumanReadable(tmx.speed()) << std::endl;
    }

}
Y_UDONE()
