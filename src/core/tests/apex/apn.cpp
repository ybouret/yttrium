

#include "y/apex/n/proto.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"

using namespace Yttrium;


namespace
{
    template <typename Core, typename Word> static inline
    void TestProto(Random::Bits &ran)
    {
        std::cerr << std::endl;
        typedef Apex::Proto<Core,Word> PROTO;
        Y_SIZEOF(PROTO);
        std::cerr << "Apex::Proto<" << PROTO::CoreSize*8 << "," << PROTO::WordSize*8 << ">" << std::endl;
        std::cerr << "Splitter::MaxWord=" << PROTO::Splitter::MaxWords << std::endl;
        {
            PROTO proto(0,AsCapacity);
        }


        for(unsigned i=0;i<=64;++i)
        {
            const uint64_t u = ran.to<uint64_t>(i);
            PROTO proto(u);
            Y_ASSERT( proto.nbits == BitsFor_(u) );
            proto.display();
            const uint64_t p = proto.ls64();
            Y_ASSERT(p==u);
        }


    }
}

Y_UTEST(apex_natural)
{
    Random::Rand ran;
    TestProto<uint64_t,uint32_t>(ran);
    TestProto<uint64_t,uint16_t>(ran);
    TestProto<uint64_t,uint8_t>(ran);

    TestProto<uint32_t,uint16_t>(ran);
    TestProto<uint32_t,uint8_t>(ran);

    TestProto<uint16_t,uint8_t>(ran);





}
Y_UDONE()
