#include "y/utest/timing.hpp"
#include "y/utest/run.hpp"
#include "y/type/utils.hpp"
#include <cmath>

using namespace Yttrium;



Y_UTEST(wtime)
{

    for(size_t i=0;i<10;++i)
    {
        std::cerr << WallTime::Ticks() << std::endl;
    }

    WallTime chrono;
    std::cerr << "freq=" << chrono.freq << std::endl;
    std::cerr << "waiting 0.1 second..." << std::endl;
    const uint64_t mark = WallTime::Ticks();
    while( chrono( WallTime::Ticks() - mark ) < 0.1 )
        ;

    std::cerr << "Timing..." << std::endl;
    Timing tmx;

    volatile double sum = 0;
    Y_Timing_Add(tmx,
                 for(unsigned i=0;i<1000;++i) sum += Squared( cos(double(i)) ),
                 1);

    std::cerr << "sum=" << sum << std::endl;
    std::cerr << "tmx.ticks=" << tmx.ticks << std::endl;
    std::cerr << "tmx.cycle=" << tmx.cycle << std::endl;
    std::cerr << "ellapsed : " << tmx(tmx.ticks) << std::endl;
    std::cerr << "speed    : " << HumanReadable(tmx.speed()) << "Op/s" << std::endl;
}
Y_UDONE()

