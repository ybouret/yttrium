
#include "y/system/exception.hpp"
#include "y/utest/run.hpp"
#include "y/system/error.hpp"

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

    //Libc::CriticalError(ENOMEM, "At the End of %s",program);

}
Y_UDONE()

