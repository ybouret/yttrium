#include "y/memory/wad.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/allocator/dyadic.hpp"

using namespace Yttrium;

Y_UTEST(memory_wad)
{

    Y_SIZEOF(Memory::Crux::Wad);

    Memory::Wad<int,Memory::Pooled> iWad(17);
    std::cerr << "iwad: maxBlocks=" << iWad.maxBlocks << " / allocated=" << iWad.allocated << std::endl;



}
Y_UDONE()
