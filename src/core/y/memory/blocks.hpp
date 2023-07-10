//! \file

#ifndef Y_Memory_Blocks_Included
#define Y_Memory_Blocks_Included 1

#include "y/memory/arena.hpp"

namespace Yttrium
{

    namespace Memory
    {

        //______________________________________________________________________
        //
        //
        //
        //! Open Hash Table of Arenas
        //
        //
        //______________________________________________________________________
        class Blocks
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ListOf<Arena>     Slot;     //!< alias
            static const char * const CallSign; //!< "Memory::Block"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Blocks(Album &);   //!< create slots
            virtual ~Blocks() noexcept; //!< clean all

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void *acquire(const size_t blockSize);                           //!< acquire a block[blockSize]
            void  release(void *blockAddr, const size_t blockSize) noexcept; //!< release a previously acquired block

            //! display statistics
            void  displayInfo(size_t indent) const;


            
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Album &      album; //!< persistent album of pages
            const size_t nslot; //!< power of two initial slots : pageSize/sizeof(Slot)
            const size_t smask; //!< nslot-1

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blocks);
            typedef void * (Blocks:: *Acquire)(const size_t);
            Arena       *cache; //!< I/O cache
            Slot        *slots; //!< slot memory
            Acquire      which; //!< acquire methods, initially acquireFirst
            Arena        build; //!< Arena to build Arenas
            
            void *  acquireFirst(const size_t blockSize);
            void *  acquireExtra(const size_t blockSize);
            Arena * makeNewArena(const size_t blockSize);
        };

    }

}

#endif

