#include "y/memory/blanks.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


Y_UTEST(memory_blanks)
{



    Blanks<int> icache(32);

    int *a = icache.zacquire();

    icache.zrelease(a);

    icache.displayInfo(0);
    icache.gc(10);
    icache.displayInfo(0);

}
Y_UDONE()
