#include "y/memory/chunk.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/memory/legacy.hpp"
#include "y/utest/run.hpp"
#include "y/calculus/align.hpp"
#include "y/calculus/base2.hpp"

#include "../alea.hpp"


using namespace Yttrium;


Y_UTEST(memory_chunk)
{
    alea_seed();

    void *addr[256];
    Y_STATIC_ZARR(addr);

    Memory::Legacy ram;

    const size_t max_block_size = 128;

    for(size_t block_size=1;block_size<=max_block_size;++block_size)
    {
        std::cerr << "[block_size=" << block_size << "]" << std::endl;
        const size_t max_blocks     = 1+alea_leq(254);
        const size_t max_chunk_size = max_blocks * block_size;
        for(size_t chunk_size=block_size;chunk_size<=max_chunk_size;chunk_size += 1+alea_leq(block_size))
        {
            void  *chunk_data = ram.acquire(chunk_size);
            Memory::Chunk chunk(block_size, chunk_data, chunk_size);
            
            size_t count = 0;

            for(size_t loop=1;loop<=10;++loop)
            {
                while(chunk.stillAvailable)
                {
                    addr[count++] = chunk.acquire(block_size);
                }
                alea_shuffle(addr,count);
                size_t half = count >> 1;
                while(half-- > 0) chunk.release(addr[--count],block_size);
            }
            bool released = false;
            while(count>0)
                released = chunk.release(addr[--count],block_size);
            Y_ASSERT(true==released);

            ram.release(chunk_data,chunk_size);
        }
    }

    Y_SIZEOF(Memory::Chunk);


    for(unsigned blockSize=1;blockSize<=max_block_size;++blockSize)
    {
        size_t optPageBytes = Memory::Chunk::OptPageBytes(blockSize);


        std::cerr << "blockSize = " << std::setw(4) << blockSize;
        std::cerr << " | optPageBytes = " << std::setw(6) << optPageBytes;
        std::cerr << " | optNumBlocks =";
        while(optPageBytes>=256)
        {
            const size_t optNumBlocks = Memory::Chunk::OptNumBlocks(blockSize,optPageBytes);
            const size_t optFlatBytes = Memory::Chunk::GetFlatBytes(blockSize,optNumBlocks);
            const size_t loss         = optPageBytes - optFlatBytes;
            std::cerr << ' ' << std::setw(4) << optNumBlocks << "(" << std::setw(3) << loss << ")";

            void          *page = ram.acquire(optPageBytes);
            Memory::Chunk *ch   = Memory::Chunk::MakeFor(blockSize,page,optPageBytes);
            Y_ASSERT(ch->providedNumber==optNumBlocks);
            ram.release(page,optPageBytes);

            optPageBytes /= 2;

        }

        std::cerr << std::endl;



    }





    

}
Y_UDONE()

