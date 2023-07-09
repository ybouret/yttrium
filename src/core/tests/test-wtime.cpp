#include "y/system/wtime.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(wtime)
{

    for(size_t i=0;i<10;++i)
    {
        std::cerr << WallTime::Ticks() << std::endl;
    }

}
Y_UDONE()

