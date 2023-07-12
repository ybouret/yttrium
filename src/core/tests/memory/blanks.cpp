#include "y/memory/blanks.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


Y_UTEST(memory_blanks)
{



    Blanks<int> icache(0);

    int *a = icache.zacquire();

    icache.zrelease(a);


}
Y_UDONE()
