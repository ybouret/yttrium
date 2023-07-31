#include "y/calculus/bit-count.hpp"
#include "y/calculus/byte-count.hpp"
#include "y/random/bits.hpp"

#include "y/utest/run.hpp"


using namespace Yttrium;

Y_UTEST(calculus_bcount)
{

    Random::Rand ran;

    std::cerr << "8 bits" << std::endl;
    {
        const uint8_t zero = 0;
        Y_CHECK(BitCount::For(zero)==0);
        Y_CHECK(ByteCount::For(zero)==0);
        for(uint64_t i=1;i<256;++i)
        {
            const uint8_t gtz(i);
            Y_ASSERT(BitCount::For(gtz)>0);
            Y_ASSERT(BitCount::For(gtz)<=8);
            Y_ASSERT(ByteCount::For(gtz)==1);
        }
    }
    std::cerr << std::endl;

    std::cerr << "16 bits" << std::endl;
    {
        const uint16_t zero = 0;
        Y_CHECK(BitCount::For(zero)==0);
        Y_CHECK(ByteCount::For(zero)==0);
        for(uint64_t i=1;i<256;++i)
        {
            const uint16_t w(i);
            Y_ASSERT(BitCount::For(w)>0);
            Y_ASSERT(BitCount::For(w)<=8);
            Y_ASSERT(ByteCount::For(w)==1);
        }
        for(uint64_t i=256;i<65536;++i)
        {
            const uint16_t w(i);
            Y_ASSERT(BitCount::For(w)>8);
            Y_ASSERT(BitCount::For(w)<=16);
            Y_ASSERT(ByteCount::For(w)==2);
        }
    }
    std::cerr << std::endl;


    std::cerr << "32 bits" << std::endl;
    {
        const uint32_t zero = 0;
        Y_CHECK(BitCount::For(zero)==0);
        Y_CHECK(ByteCount::For(zero)==0);
        for(uint64_t i=1;i<256;++i)
        {
            const uint32_t dw = uint32_t(i);
            Y_ASSERT(BitCount::For(dw)>0);
            Y_ASSERT(BitCount::For(dw)<=8);
            Y_ASSERT(ByteCount::For(dw)==1);
        }
        for(uint64_t i=256;i<65536;++i)
        {
            const uint32_t dw = uint32_t(i);
            Y_ASSERT(BitCount::For(dw)>8);
            Y_ASSERT(BitCount::For(dw)<=16);
            Y_ASSERT(ByteCount::For(dw)==2);
        }

        for(uint64_t i=65536;i<16777216;++i)
        {
            const uint32_t dw = uint32_t(i);
            Y_ASSERT(BitCount::For(dw)>16);
            Y_ASSERT(BitCount::For(dw)<=24);
            Y_ASSERT(ByteCount::For(dw)==3);
        }

        uint32_t test = 16777216;
        Y_CHECK(25 == BitCount::For(test));
        Y_CHECK(4  == ByteCount::For(test));
        test = 0xffffffff;
        Y_CHECK(32 == BitCount::For(test));
        Y_CHECK(4  == ByteCount::For(test));

        for(size_t iter=0;iter<1000;++iter)
        {
            test = ran.in<uint32_t>(16777216, 0xffffffff);
            Y_ASSERT(BitCount::For(test)>24);
            Y_ASSERT(BitCount::For(test)<=32);
            Y_ASSERT(4  == ByteCount::For(test));
        }


    }
    std::cerr << std::endl;


    std::cerr << "64 bits" << std::endl;
    {
        const uint64_t zero = 0;
        Y_CHECK(BitCount::For(zero)==0);
        Y_CHECK(ByteCount::For(zero)==0);
        for(uint64_t i=1;i<256;++i)
        {
            Y_ASSERT(BitCount::For(i)>0);
            Y_ASSERT(BitCount::For(i)<=8);
            Y_ASSERT(ByteCount::For(i)==1);
        }
        for(uint64_t i=256;i<65536;++i)
        {
            Y_ASSERT(BitCount::For(i)>8);
            Y_ASSERT(BitCount::For(i)<=16);
            Y_ASSERT(ByteCount::For(i)==2);
        }

        for(uint64_t i=65536;i<16777216;++i)
        {
            Y_ASSERT(BitCount::For(i)>16);
            Y_ASSERT(BitCount::For(i)<=24);
            Y_ASSERT(ByteCount::For(i)==3);
        }
        uint64_t test = 16777216;
        Y_CHECK(25 == BitCount::For(test));
    }
    std::cerr << std::endl;

}
Y_UDONE()
