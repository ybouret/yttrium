#include "y/random/bits.hpp"
#include "y/utest/run.hpp"
#include "y/check/crc32.hpp"

#include <ctime>

using namespace Yttrium;




Y_UTEST(random_bits)
{
    const time_t tm = time(0);
    srand( CRC32::Of(tm) );

    Y_SIZEOF(Random::Bits);

    Random::Rand ran;

    for(size_t i=0;i<10;++i)
    {
        std::cerr << ran.to<double>() << std::endl;
        std::cerr << int(ran.to<uint8_t>()) << std::endl;
    }





}
Y_UDONE()

