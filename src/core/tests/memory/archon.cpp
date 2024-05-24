#include "y/memory/allocator/archon.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(memory_archon)
{
    unsigned shift = 0;
    void    *block = Memory::Archon::Acquire(shift);
    std::cerr << "block@" << block << ", shift=" << shift << std::endl;
    Y_CHECK(shift>=Memory::Archon::MinShift);
    Memory::Archon::Release(block,shift);
}
Y_UDONE()

