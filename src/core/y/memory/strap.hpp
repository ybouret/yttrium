
//! \file

#ifndef Y_Memory_Strap_Included
#define Y_Memory_Strap_Included 1

#include "y/calculus/ilog2.hpp"
#include "y/type/ints.hpp"

namespace Yttrium
{

    namespace Memory
    {

        class Strap
        {
        public:
            typedef UnsignedInt<sizeof(void*)>::Type len_t;

            struct Block
            {
                Block *next;
                Block *prev;
                Strap *used;
                len_t  size;
            };

            static const size_t   BlockBytes = sizeof(Block);
            static const unsigned BlockShift = iLog2<BlockBytes>::Value;
            static const size_t   MinRequest = 4 * BlockBytes;


            Strap(void *addr, const size_t size) noexcept;
            ~Strap() noexcept;

            static size_t BlockSize(const size_t blockSize) noexcept;
            
            void *acquire(size_t &blockSize) noexcept;


            Strap *next;
            Strap *prev;
            Block *head;
            Block *tail;



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Strap);
        };

    }

}

#endif

