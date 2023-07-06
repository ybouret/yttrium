//! \file

#ifndef Y_Memory_Strap_Included
#define Y_Memory_Strap_Included 1

#include "y/calculus/ilog2.hpp"

namespace Yttrium
{

    namespace Memory
    {

        struct Block;

        class Strap
        {
        public:
            Strap(void *addr, const size_t size) noexcept;
            ~Strap() noexcept;


            void *        acquire(size_t &blockSize) noexcept;
            static void   Release(void *  blockAddr) noexcept;
            size_t        shift__() const noexcept;

            static size_t BlockSizeFor(const size_t blockSize) noexcept;
            static size_t ShiftToHold(const size_t blockSize);


            void   displayInfo(size_t indent) const;

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

