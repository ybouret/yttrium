

#include "y/memory/album.hpp"
#include "y/utest/run.hpp"
#include "../alea.hpp"
#include "y/lockable.hpp"

using namespace Yttrium;


Y_UTEST(memory_album)
{
    alea_seed();

    Y_SIZEOF(Memory::Album);

    Memory::Album album;


}
Y_UDONE()
