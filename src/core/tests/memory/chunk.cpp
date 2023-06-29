#include "y/memory/chunk.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/memory/ram.hpp"
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
    std::cerr << "sizeof(Memory::Chunk)=" << sizeof(Memory::Chunk) << std::endl;

    void *addr[256];
    Y_STATIC_ZARR(addr);

    const size_t max_block_size = 40;
    for(size_t block_size=1;block_size<=max_block_size;++block_size)
    {
        std::cerr << "[block_size=" << block_size << "]" << std::endl;
        const size_t max_blocks     = 1+alea_leq(254);
        const size_t max_chunk_size = max_blocks * block_size;
        for(size_t chunk_size=block_size;chunk_size<=max_chunk_size;chunk_size += 1+alea_leq(block_size))
        {
            void *chunk_data = Memory::RAM::Acquire(chunk_size);
            Memory::Chunk chunk(block_size, chunk_data, chunk_size);
            
            size_t count = 0;

            for(size_t loop=1;loop<=10;++loop)
            {
                while(chunk.still_available)
                {
                    addr[count++] = chunk.acquire(block_size);
                }
                alea_shuffle(addr,count);
                size_t half = count >> 1;
                while(half-- > 0) chunk.release(addr[--count],block_size);
            }
            bool res = 0;
            while(count>0)
                res = chunk.release(addr[--count],block_size);
            if(!res) throw std::exception();

            Memory::RAM::Release(chunk_data,chunk_size);
        }
    }

    

}
Y_UDONE()

