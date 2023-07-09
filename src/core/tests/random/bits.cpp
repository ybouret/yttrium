#include "y/random/park-miller.hpp"
#include "y/system/seed.hpp"
#include "y/utest/run.hpp"
#include "y/check/crc32.hpp"

#include <ctime>

using namespace Yttrium;




Y_UTEST(random_bits)
{
    const time_t   tm   = time(0);
    const uint32_t seed = CRC32::Of(tm);
    srand(seed);

    Y_SIZEOF(Random::Bits);

    Random::Rand       ran1;
    Random::ParkMiller ran2(seed);

    for(size_t i=0;i<10;++i)
    {
        std::cerr << ran1.to<double>() << std::endl;
    }


    for(size_t i=0;i<10;++i)
    {
        std::cerr << ran2.to<double>() << std::endl;
    }

    for(size_t i=0;i<10;++i)
    {
        std::cerr << "seed=" << SystemSeed::Get() << std::endl;
    }
    

}
Y_UDONE()

