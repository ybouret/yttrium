
#include "y/utest/run.hpp"

#if defined(Y_BSD)
#include <sys/select.h>
#endif

#if defined(Y_WIN)
#include <winsock2.h>
#endif

using namespace Yttrium;

Y_UTEST(eta)
{
    struct timeval tv = { 1, 200 };

    const int res = select(0, 0, 0, 0, &tv);

    std::cerr << "res=" << res << std::endl;

}
Y_UDONE()

