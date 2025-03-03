
#include "y/system/stopwatch.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(stopwatch)
{

    uint64_t ell = 0;
    {
        Y_StopWatch(&ell);
        std::cerr << "Printing something" << std::endl;
    }
    {
        Y_StopWatch(0);
        std::cerr << "Printing again..." << std::endl;
    }


    WallTime chrono;
    std::cerr << "Ellapsed: " << chrono(ell) << std::endl;

}
Y_UDONE()

