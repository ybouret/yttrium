

#include "y/apex/n/proto.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"

using namespace Yttrium;


Y_UTEST(apex_natural)
{
    Apex::Proto<uint64_t,uint32_t> p64_32; Y_USHOW(p64_32.data.bytes);
    Apex::Proto<uint64_t,uint16_t> p64_16; Y_USHOW(p64_16.data.bytes);
    Apex::Proto<uint64_t,uint8_t>  p64_8;  Y_USHOW(p64_8.data.bytes);

}
Y_UDONE()
