

#include "y/memory/guild.hpp"
#include "y/memory/pages.hpp"
#include "y/memory/album.hpp"

#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"

#include <cstring>

using namespace Yttrium;


Y_UTEST(memory_arena)
{
    Random::Rand ran;
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
        Memory::Arena arena(blockSize,album);

        arena.releaseBlock( arena.acquireBlock() );
        

        memset(blk,0,sizeof(blk));
        for(size_t i=0;i<NB;++i)
        {
            blk[i] = arena.acquireBlock();
        }

        album.displayInfo(0);
        arena.displayInfo(0);
        
        for(size_t loop=1;loop<=4;++loop)
        {
            Random::Shuffle::Range(blk,NB,ran);
            for(size_t i=0;i<NB/2;++i)
            {
                arena.releaseBlock(blk[i]);
            }
            for(size_t i=0;i<NB/2;++i)
            {
                blk[i] = arena.acquireBlock();
            }
        }

        std::cerr << "-------- Full Usage: " << std::endl;
        album.displayInfo(0);
        arena.displayInfo(0);
        Random::Shuffle::Range(blk,NB,ran);
        for(size_t i=0;i<NB;++i)
        {
           arena.releaseBlock( blk[i] );
        }

        std::cerr << "-------- Empty: " << std::endl;
        album.displayInfo(0);
        arena.displayInfo(0);
    }

    std::cerr << "Final: " << std::endl;
    album.displayInfo(0);

    Y_SIZEOF(Memory::Arena);
    Memory::Arena selfArena(sizeof(Memory::Arena),album);
    selfArena.displayInfo(0);


    for(size_t blockSize = 4; blockSize <= 512; blockSize += 4)
    {
        Memory::Arena arena(blockSize,album);
        std::cerr << "Arena[" << std::setw(4) << blockSize << "] : pageSize=" << std::setw(6) << arena.dataPages.bytes <<  " | BPP = " << arena.numBlocks << std::endl;
        if( arena.dataPages.bytes > Memory::Page::DefaultBytes)
            break;
    }

}
Y_UDONE()
