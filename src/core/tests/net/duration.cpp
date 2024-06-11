
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
}
Y_UDONE()
