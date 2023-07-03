

#include "y/memory/arena.hpp"
#include "y/memory/pages.hpp"
#include "y/memory/album.hpp"

#include "y/utest/run.hpp"
#include "../alea.hpp"

#include <cstring>

using namespace Yttrium;


Y_UTEST(memory_arena)
{
    alea_seed();

    Y_SIZEOF(Memory::Arena);

    size_t pageBytes = 4096;

    for(size_t blockSize = 1; blockSize <= 256; ++blockSize )
    {
        size_t         nblks = 0;
        const unsigned shift = Memory::Arena::ComputeShift(blockSize, pageBytes, nblks);
        const size_t   bytes = Base2<size_t>::One << shift;
        const size_t   loss  = bytes - (sizeof(Memory::Chunk) + nblks * blockSize);
        std::cerr << "blockSize = " << std::setw(4) << blockSize
        << " : pageSize = " << std::setw(6) << pageBytes
        << " -> "           << std::setw(6) << bytes << " : #" << std::setw(4) <<  (nblks) << " | loss=" << loss << std::endl;
    }

    Memory::Album album;

    std::cerr << "Using Arena" << std::endl;

    const size_t NB = 10000;
    void *blk[NB];

    for(size_t blockSize = 1; blockSize <= 64; blockSize += 1)
    {
        Memory::Arena arena(blockSize,album,pageBytes);
        memset(blk,0,sizeof(blk));


        for(size_t i=0;i<NB;++i)
        {
            blk[i] = arena.acquire();
        }

        album.displayInfo(0);
        arena.displayInfo(0);
        
        for(size_t loop=1;loop<=4;++loop)
        {
            alea_shuffle(blk,NB);
            for(size_t i=0;i<NB/2;++i)
            {
                arena.release(blk[i]);
            }
            for(size_t i=0;i<NB/2;++i)
            {
                blk[i] = arena.acquire();
            }
        }

        std::cerr << "-------- Full Usage: " << std::endl;
        album.displayInfo(0);
        arena.displayInfo(0);
        alea_shuffle(blk,NB);
        for(size_t i=0;i<NB;++i)
        {
           arena.release( blk[i] );
        }

        std::cerr << "-------- Empty: " << std::endl;
        album.displayInfo(0);
        arena.displayInfo(0);
    }

    std::cerr << "Final: " << std::endl;
    album.displayInfo(0);



}
Y_UDONE()
