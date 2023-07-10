//! \file

#ifndef Y_Memory_Quanta_Included
#define Y_Memory_Quanta_Included 1

#include "y/memory/album.hpp"

namespace Yttrium
{

    namespace Memory
    {
        class Blocks;
        class Straps;

        class Quanta
        {
        public:
            static const size_t LimitSize = 256;
            
            explicit Quanta(Blocks &, Straps &) noexcept;
            virtual ~Quanta() noexcept;

            void * acquire(size_t blockSize);
            void   release(void *blockAddr, const size_t blockSize) noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Quanta);
            Blocks &blocks;
            Straps &straps;
        };
    }

}

#endif

