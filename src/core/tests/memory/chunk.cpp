#include "y/memory/chunk.h"
#include "y/memory/out-of-reach.h"
#include "y/utest/run.hpp"

#include <cmath>

using namespace Yttrium;

namespace
{
    double alea()
    {
        return (0.5 + rand()) / (1.0+RAND_MAX);
    }

    size_t alea_leq(const size_t n)
    {
        return static_cast<size_t>(floor(double(n) * alea() + 0.5));
    }

    template <typename T>
    void alea_shuffle(T a[], const size_t n)
    {
        for(size_t i=n-1;i>0;--i)
        {
            const size_t j = alea_leq(i);
            Swap(a[i], a[j]);
        }
    }

}

Y_UTEST(memory_chunk)
{
    Y_MemoryChunk chunk = Y_MemoryChunk_Static_Init;

    Y_OutOfReach_Init(chunk);

    std::cerr << "sizeof(chunk)=" << sizeof(Y_MemoryChunk) << std::endl;

    void *addr[256];
    Y_OutOfReach_Zero(addr,sizeof(addr));

    const size_t max_block_size = 40;
    for(size_t block_size=1;block_size<=max_block_size;++block_size)
    {
        std::cerr << "[block_size=" << block_size << "]" << std::endl;
        const size_t max_blocks     = 1+alea_leq(254);
        const size_t max_chunk_size = max_blocks * block_size;
        for(size_t chunk_size=block_size;chunk_size<=max_chunk_size;chunk_size += 1+alea_leq(block_size))
        {
            void *chunk_data = calloc(1, chunk_size);
            if(!chunk_data) throw std::exception();
            Y_MemoryChunk_Init(&chunk, block_size, chunk_data, chunk_size);
            
            size_t count = 0;

            for(size_t loop=1;loop<=10;++loop)
            {
                while(chunk.still_available)
                {
                    addr[count++] = Y_MemoryChunk_Acquire(&chunk,block_size);
                }
                alea_shuffle(addr,count);
                size_t half = count >> 1;
                while(half-- > 0) Y_MemoryChunk_Release(&chunk,addr[--count],block_size);
            }
            int res = 0;
            while(count>0)
                res = Y_MemoryChunk_Release(&chunk,addr[--count],block_size);
            if(!res) throw std::exception();

            Y_MemoryChunk_Quit(&chunk,block_size);



            free(chunk_data);
        }
    }

    

}
Y_UDONE()

