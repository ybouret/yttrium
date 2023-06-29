#include "y/memory/ram.hpp"
#include <cstdlib>
#include <exception>

namespace Yttrium
{

    namespace Memory
    {
        static uint64_t AllocatedRAM = 0x00;

        void * RAM:: Acquire(size_t block_size)
        {
            if(block_size<=0)
            {
                return 0;
            }
            else
            {
                void *res = calloc(1,block_size);
                if(!res)
                    throw std::exception();

                AllocatedRAM += block_size;
                return res;
            }
        }

        void RAM:: Release(void *block_addr, const size_t block_size) noexcept
        {
            assert(Y_Good(block_addr,block_size));
            if(0 != block_addr)
            {
                assert(block_size>0);
                free(block_addr);
                AllocatedRAM -= block_size;
            }
        }
    }

}

