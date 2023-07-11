
#include "y/memory/quanta.hpp"
#include "y/memory/straps.hpp"
#include "y/memory/blocks.hpp"
#include "y/utest/run.hpp"
#include "y/data/list/cxx.hpp"
#include "y/random/shuffle.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/data/list/cxx.hpp"

using namespace Yttrium;


namespace
{
    class block_t
    {
    public:
        block_t *next;
        block_t *prev;
        void    *addr;
        size_t   size;

        inline block_t() noexcept : next(0), prev(0), addr(0), size(0)
        {
        }

        inline ~block_t() noexcept
        {
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(block_t);
    };

}


Y_UTEST(memory_quanta)
{

    Random::Rand ran;

    Y_SIZEOF(Memory::Album);
    Y_SIZEOF(Memory::Blocks);
    Y_SIZEOF(Memory::Straps);
    Y_SIZEOF(Memory::Quanta);

    Memory::Album  album;
    Memory::Blocks blocks(album);
    Memory::Straps straps(album);
    Memory::Quanta quanta(blocks,straps);


    CxxListOf<block_t> L;
    for(size_t blockSize=1; blockSize <= 1000; blockSize += ran.leq(10) )
    {
        block_t *blk = L.pushTail(new block_t());
        blk->size = blockSize;
        blk->addr = quanta.acquire(blk->size);
        std::cerr << "/" << blockSize;
    }
    std::cerr << std::endl;

    Random::Shuffle::List(L,ran);
    while( L.size )
    {
        block_t *blk = L.popTail();
        std::cerr << "/" << blk->size;
        quanta.release(blk->addr,blk->size);
    }
    std::cerr << std::endl;
    straps.displayInfo(0);
    album.displayInfo(0);

}
Y_UDONE()


