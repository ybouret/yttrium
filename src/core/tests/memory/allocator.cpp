


#include "y/memory/allocator/global.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/utest/run.hpp"
#include "y/lockable.hpp"

using namespace Yttrium;


Y_UTEST(memory_allocator)
{

    Memory::Allocator &G = Memory::Global::Instance();
    std::cerr << G.variety() << std::endl;

    {
        size_t count = 1;
        void  *entry = G.acquire(count,10);
        std::cerr << "count=" << count << std::endl;
        G.release(entry,count);
    }

    Memory::Allocator &P = Memory::Pooled::Instance();
    std::cerr << P.variety() << std::endl;

    {
        size_t count = 1;
        void  *entry = P.acquire(count,10);
        std::cerr << "count=" << count << std::endl;
        P.release(entry,count);
    }




}
Y_UDONE()
