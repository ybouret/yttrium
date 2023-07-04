//! \file

#ifndef Y_Memory_Blocks_Included
#define Y_Memory_Blocks_Included 1

#include "y/memory/arena.hpp"

namespace Yttrium
{

    namespace Memory
    {

        class Blocks
        {
        public:
            typedef void * (Blocks:: *Acquire)(const size_t);

            typedef ListOf<Arena>     Slot;
            static const char * const CallSign;

            explicit Blocks(Album &);
            virtual ~Blocks() noexcept;

            void *acquire(const size_t blockSize);
            void  release(void *blockAddr, const size_t blockSize) noexcept;
            
            void  displayInfo(size_t indent) const;


            Album &      album;
            const size_t nslot;
            const size_t smask;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blocks);
            Arena       *cache;
            Slot        *slots;
            Acquire      which;
            Arena        build;
            
            void *  acquireFirst(const size_t blockSize);
            void *  acquireExtra(const size_t blockSize);
            Arena * makeNewArena(const size_t blockSize);
        };

    }

}

#endif

