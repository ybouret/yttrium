#include "y/memory/ram.hpp"
#include "y/system/exception.hpp"
#include "y/type/addition.hpp"
#include "y/system/error.hpp"
#include "y/concurrent/mutex.hpp"
#include <cstdlib>
#include <cerrno>
#include <iostream>

namespace Yttrium
{

    namespace Memory
    {
        static uint64_t AllocatedRAM = 0x00;


        RAM::  RAM() : Allocator(), access( Concurrent::Mutex::Giant() ) {}
        RAM:: ~RAM() noexcept
        {
            if(AllocatedRAM)
            {
                std::cerr << "[RAM : " << AllocatedRAM << "]" << std::endl;
            }
        }


        const char * const RAM:: CallSign = "RAM";

        const char * RAM:: variety() const noexcept { return CallSign; }

        void * RAM:: acquire(size_t &count, const size_t blockSize)
        {
            const ScopedLock guard(access);

            size_t required = count * blockSize;
            if(required<=0)
            {
                return 0;
            }
            else
            {
                const uint64_t AllocatedNew = Addition::Of(AllocatedRAM,required, "of RAM");
                void *res = calloc(1,required);
                if(!res)
                    throw Libc::Exception(ENOMEM,"RAM::Acquire(%lu)", (unsigned long)required);

                AllocatedRAM = AllocatedNew;
                count = required;
                return res;
            }
        }

        void RAM:: release(void * &entry, size_t & count) noexcept
        {
            const ScopedLock guard(access);

            assert(Good(entry,count));
            if(0 != entry)
            {
                assert(count>0);
                if(count>AllocatedRAM)
                {
                    Libc::CriticalError(ERANGE,
                                        "RAM::Releasing(count=%lu>RAM=%lu)",
                                        (unsigned long)count,
                                        (unsigned long)AllocatedRAM);
                }
                free(entry);
                AllocatedRAM -= count;
                entry = 0;
                count = 0;
            }
        }

        uint64_t RAM:: Allocated() noexcept { return AllocatedRAM; }
    }

}

