


#include "y/memory/album.hpp"
#include "y/memory/blocks.hpp"

#include "y/utest/run.hpp"
#include "../alea.hpp"

#include <cstring>

using namespace Yttrium;


Y_UTEST(memory_blocks)
{

    Memory::Album  album;
    Memory::Blocks blocks(album);
    blocks.expand();
    
}
Y_UDONE()
