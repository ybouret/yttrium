#include "y/system/wtime.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(wtime)
{

    for(size_t i=0;i<10;++i)
    {
        std::cerr << WallTime::Ticks() << std::endl;
    }

    WallTime chrono;
    std::cerr << "freq=" << chrono.freq << std::endl;
    std::cerr << "waiting 1 second..." << std::endl;
    const uint64_t mark = WallTime::Ticks();
    while( chrono( WallTime::Ticks() - mark ) < 1.0 )
        ;



}
Y_UDONE()

