
#include "y/system/exception.hpp"
#include "y/utest/run.hpp"

#include <cmath>
#include <cerrno>

using namespace Yttrium;

Y_UTEST(excp)
{
    Y_SIZEOF(std::exception);
    Y_SIZEOF(Exception);

    {
        Libc::Exception excp(ENOMEM,"Trial @%s",program);
        excp.display();
    }

}
Y_UDONE()

