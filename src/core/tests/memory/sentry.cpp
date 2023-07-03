

#include "y/memory/sentry.hpp"
#include "y/utest/run.hpp"
#include "y/memory/out-of-reach.hpp"

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

    char c[64];
    Memory::OutOfReach::Zero(c,sizeof(c));

    for(char i='a';i<='z';++i) c[i-'a'] = i;
    std::cerr << "c=" << c << std::endl;

    Memory::OutOfReach::Lift(c,26,7);
    std::cerr << "c=" << c+7 << std::endl;
    Y_ASSERT(Memory::OutOfReach::Are0(c,7));





}
Y_UDONE()
