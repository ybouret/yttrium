#include "y/stream/bits.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(io_bits)
{
    StreamBits io;
    io.push(171,8);

    std::cerr << io << std::endl;

    const unsigned u = io.pull<unsigned>(8);
    std::cerr << u << std::endl;

}
Y_UDONE()
