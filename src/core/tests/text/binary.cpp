
#include "y/text/binary.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"

using namespace Yttrium;



Y_UTEST(text_binary)
{
    Random::Rand ran;

    const uint16_t u = 0x3;
    std::cerr << Binary(u,Binary::Compact) << std::endl;
    
}
Y_UDONE()

