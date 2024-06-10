
#include "y/utest/run.hpp"
#include <sys/select.h>

using namespace Yttrium;

Y_UTEST(eta)
{
    struct timeval tv = { 1, 200 };

    const int res = select(0, 0, 0, 0, &tv);

    std::cerr << "res=" << res << std::endl;

}
Y_UDONE()

