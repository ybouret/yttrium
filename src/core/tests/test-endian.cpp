#include "y/system/endian.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(endian)
{
    std::cerr << Endianness::Text( Endianness::Detect() ) << std::endl;
}
Y_UDONE()

