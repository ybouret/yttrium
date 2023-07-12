


#include "y/memory/zcache.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


Y_UTEST(memory_zcache)
{

    Y_SIZEOF(Memory::ZombieCache);


    ZombieCache<int> icache;

    int *a = icache.acquire();

    icache.release(a);


}
Y_UDONE()
