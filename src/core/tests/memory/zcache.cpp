


#include "y/memory/zcache.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


Y_UTEST(memory_zcache)
{

    Y_SIZEOF(Memory::ZombieCache);


    ZombieCache<int> icache(0);

    int *a = icache.zacquire();

    icache.zrelease(a);


}
Y_UDONE()
