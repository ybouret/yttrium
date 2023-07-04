


#include "y/memory/album.hpp"
#include "y/memory/blocks.hpp"
#include "y/memory/out-of-reach.hpp"

#include "y/utest/run.hpp"
#include "../alea.hpp"

#include <cstring>

using namespace Yttrium;

struct block_t
{
    void  *addr;
    size_t size;
};

Y_UTEST(memory_blocks)
{
    alea_seed();

    Memory::Album  album;
    Memory::Blocks blocks(album);

    std::cerr << "#slots=" << blocks.nslot << std::endl;

    block_t      blk[10]; Y_STATIC_ZARR(blk);
    const size_t num = sizeof(blk)/sizeof(blk[0]);

    for(size_t i=0;i<num;++i)
    {
        block_t &b = blk[i];
        b.size = 1 + alea_leq(255);
        b.addr = blocks.acquire(b.size);
    }

}
Y_UDONE()
