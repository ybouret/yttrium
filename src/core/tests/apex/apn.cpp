

#include "y/apex/n/proto.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"

using namespace Yttrium;


namespace
{
    template <typename Core, typename Word> static inline
    void TestProto()
    {
        std::cerr << std::endl;
        typedef Apex::Proto<Core,Word> PROTO;
        Y_SIZEOF(PROTO);
        std::cerr << "Apex::Proto<" << PROTO::CoreSize*8 << "," << PROTO::WordSize*8 << ">" << std::endl;
        std::cerr << "Splitter::MaxWord=" << PROTO::Splitter::MaxWords << std::endl;
        {
            PROTO proto(0,AsCapacity);
        }

        const uint64_t U[] =
        {
            0x00,
            0x06,
            0xab,
            0xabc,
            0xabcd,
            0xabcde,
            0xabcdef,
            0xabcdef12,
            Y_U64(0x12345678abcdef00)
        };

        for(size_t i=0;i<sizeof(U)/sizeof(U[0]);++i)
        {
            const uint64_t u = U[i];
            PROTO proto(u);
            Y_ASSERT( proto.nbits == BitsFor_(u) );
            proto.display();
        }


    }
}

Y_UTEST(apex_natural)
{
    TestProto<uint64_t,uint32_t>();
    TestProto<uint64_t,uint16_t>();
    TestProto<uint64_t,uint8_t>();

    TestProto<uint32_t,uint16_t>();
    TestProto<uint32_t,uint8_t>();

    TestProto<uint16_t,uint8_t>();





}
Y_UDONE()
