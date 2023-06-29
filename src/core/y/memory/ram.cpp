#include "y/memory/ram.hpp"
#include "y/system/exception.hpp"
#include "y/type/addition.hpp"
#include "y/system/error.hpp"
#include <cstdlib>
#include <cerrno>

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
                const uint64_t AllocatedNew = Addition::Of(AllocatedRAM,block_size, "of RAM");
                void *res = calloc(1,block_size);
                if(!res)
                    throw Libc::Exception(ENOMEM,"RAM::Acquire(%lu)", (unsigned long)block_size);

                AllocatedRAM = AllocatedNew;
                return res;
            }
        }

        void RAM:: Release(void *block_addr, const size_t block_size) noexcept
        {
            assert(Good(block_addr,block_size));
            if(0 != block_addr)
            {
                assert(block_size>0);
                if(block_size>AllocatedRAM)
                {
                    Libc::CriticalError(ERANGE,
                                        "RAM::Releasing(block_size=%lu>RAM=%lu)",
                                        (unsigned long)block_size,
                                        (unsigned long)AllocatedRAM);
                }
                free(block_addr);
                AllocatedRAM -= block_size;
            }
        }

        uint64_t RAM:: Allocated() noexcept { return AllocatedRAM; }
    }

}

