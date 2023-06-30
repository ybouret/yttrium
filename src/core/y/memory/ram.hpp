//! \file

#ifndef Y_Memory_RAM_Included
#define Y_Memory_RAM_Included 1

#include "y/memory/allocator.hpp"

namespace Yttrium
{
    class Lockable;
    
    namespace Memory
    {

        class RAM : public Allocator
        {
        public:
            static const char * const CallSign;
            
            explicit RAM();
            virtual ~RAM() noexcept;
            

            virtual void    *    acquire(size_t &count, const size_t blockSize);
            virtual void         release(void * &entry, size_t &     count) noexcept;
            virtual const char * variety() const noexcept;

            static uint64_t Allocated() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RAM);
            Lockable &access;
        };

    }
    
}

#endif

