
#include "y/system/duration.hpp"
#include "y/utest/run.hpp"

#if defined(Y_WIN)
#include <winsock2.h>
#endif

using namespace Yttrium;

Y_UTEST(net_duration)
{

    Duration d = 1.2;
    std::cerr << d.status() << std::endl;
    std::cerr << double(d)  << std::endl;
    struct timeval * tv = d.tv();
    if(tv)
    {
        std::cerr << tv->tv_sec << " / " << tv->tv_usec << std::endl;
    }
}
Y_UDONE()
