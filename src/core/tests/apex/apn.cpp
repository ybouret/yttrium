

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
        std::cerr << "Apex::Proto<" << PROTO::CoreSize*8 << "," << PROTO::WordSize*8 << ">" << std::endl;
        PROTO proto(0,AsCapacity);

        typename PROTO::  PartType  s8(0xab);
        typename PROTO::  PartType s16(0xabcd);
        typename PROTO::  PartType s32(0xabcdef12);


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
