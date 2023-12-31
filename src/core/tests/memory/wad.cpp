#include "y/memory/wad.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/allocator/dyadic.hpp"

using namespace Yttrium;

Y_UTEST(memory_wad)
{

    Y_SIZEOF(Memory::Crux::Wad);

    Memory::Wad<int,Memory::Pooled> iWad(17);
    std::cerr << "iwad: capacity=" << iWad.capacity << " / numBytes=" << iWad.numBytes << std::endl;



}
Y_UDONE()
