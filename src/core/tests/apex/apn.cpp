

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

        // default
        {
            PROTO proto(0,AsCapacity);
            const PROTO cpy(proto);
        }

        // test 64 bits
        for(unsigned i=0;i<=64;++i)
        {
            const uint64_t u = ran.to<uint64_t>(i);
            PROTO proto(u);
            Y_ASSERT( proto.nbits == BitsFor(u) );
            proto.display();
            const uint64_t p = proto.ls64();
            Y_ASSERT(p==u);
            const PROTO copy(proto);
        }

        {
            const Word W[] = { 1,2,3,4,5 };
            PROTO proto(W,sizeof(W)/sizeof(W[0]));
            proto.display();
        }

        {
            const uint64_t l = ran.to<uint64_t>( unsigned(ran.leq(63)) );
            const uint64_t r = ran.to<uint64_t>( unsigned(ran.leq(63)) );
            const uint64_t s = l+r;
            PROTO L( l );
            PROTO R( r );
            typename PROTO::Splitter ls(l);
            typename PROTO::Splitter rs(r);
            PROTO   *pS = PROTO::Add(ls.w,ls.n,rs.w,rs.n);
            //const uint64_t S = pS->ls64();
            //std::cerr << Hexadecimal(s) << "=>" << Hexadecimal(S) << std::endl;
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
