
//! \file

#ifndef Yttrium_Memory_Allocator_Included
#define Yttrium_Memory_Allocator_Included 1


#include "y/config/starting.hpp"

namespace Yttrium
{
    
    namespace Memory
    {
        
        class Allocator
        {
        public:
            virtual ~Allocator() noexcept;

            virtual void *        acquire(size_t & count, const size_t blockSize)      = 0;
            virtual void          release(void * & entry, size_t &     count) noexcept = 0;
            virtual const char *  variety()                             const noexcept = 0;

        protected:
            explicit Allocator() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Allocator);
        };
    }
}

#endif

