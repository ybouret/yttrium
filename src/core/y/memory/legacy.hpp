
//! \file

#ifndef Y_Memory_Legacy_Included
#define Y_Memory_Legacy_Included 1

#include "y/memory/allocator.hpp"

namespace Yttrium
{
    class Lockable;

    namespace Memory
    {

        class Legacy
        {
        public:
            explicit Legacy();
            virtual ~Legacy() noexcept;

            void *acquire(const size_t blockSize);
            void  release(void *blockAddr, const size_t blockSize) noexcept;
            
            static  uint64_t Allocated() noexcept;

            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Legacy);
            Lockable &giant;
        };

    }

}

#endif

