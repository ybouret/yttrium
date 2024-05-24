#include "y/memory/allocator/archon.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(memory_archon)
{
    unsigned shift = 0;
    void    *block = Memory::Archon::Acquire(shift);
    std::cerr << "block@" << block << ", shift=" << shift << std::endl;
    Memory::Archon::Release(block,shift);
}
Y_UDONE()

