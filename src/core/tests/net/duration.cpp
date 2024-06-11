
#include "y/system/duration.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(net_duration)
{

    Duration d;
    struct timeval *tv = d.tv();
    std::cerr << tv->tv_sec << " / " << tv->tv_usec << std::endl;

}
Y_UDONE()
