


#include "y/memory/allocator/global.hpp"
#include "y/utest/run.hpp"
#include "y/lockable.hpp"

using namespace Yttrium;


Y_UTEST(memory_allocator)
{

    Memory::Allocator &G = Memory::Global::Instance();
    std::cerr << G.variety() << std::endl;

    size_t count = 1;
    void  *entry = G.acquire(count,10);

    G.release(entry,count);
    


}
Y_UDONE()
