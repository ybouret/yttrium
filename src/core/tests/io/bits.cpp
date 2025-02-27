#include "y/stream/bits.hpp"
#include "y/utest/run.hpp"

#include "y/calculus/bit-count.hpp"
#include "y/calculus/byte-count.hpp"

using namespace Yttrium;

Y_UTEST(io_bits)
{
    StreamBits io;
    io.push(171,8);

    std::cerr << io << std::endl;

    const unsigned u = io.pull<unsigned>(8);
    std::cerr << u << std::endl;


    Y_USHOW(BitsFor(64));
    Y_USHOW(BitsFor(4));

}
Y_UDONE()
