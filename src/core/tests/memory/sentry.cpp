

#include "y/memory/sentry.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


Y_UTEST(memory_sentry)
{

    int myData = 0;
    {
        Y_SENTRY(&myData,sizeof(myData));
    }

    {
        const Memory::Sentry sentry(&myData,sizeof(myData));
        Y_CHECK(sentry.isUntouched());

        myData = 1;

        Y_CHECK(sentry.wasModified());



    }


}
Y_UDONE()
