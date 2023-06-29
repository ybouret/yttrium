
//! \file

#ifndef Y_Memory_RAM_Included
#define Y_Memory_RAM_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Memory
    {

        struct RAM
        {
            static void    *Acquire(size_t block_size);
            static void     Release(void *block_addr, const size_t block_size) noexcept;
            static uint64_t Allocated() noexcept;
        };

    }
    
}

#endif

