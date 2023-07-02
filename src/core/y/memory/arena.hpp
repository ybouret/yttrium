
//! \file

#ifndef Y_Memory_Arena_Included
#define Y_Memory_Arena_Included 1

#include "y/memory/chunk.hpp"
#include "y/data/list.hpp"

namespace Yttrium
{

    namespace Memory
    {

        class Pages;
        class Album;

        class Arena : public ListOf<Chunk>
        {
        public:
            static const uint8_t      MinBlocksPerChunk = 16;
            static const uint8_t      MaxBlocksPerChunk = 255;
            static const char * const CallSign;

            explicit Arena(const size_t  userBlockSize,
                           Album        &userDataPages,
                           const size_t  userpageBytes);
            
            virtual ~Arena() noexcept;

            static unsigned ComputeShift(const size_t blockSize,
                                         const size_t pageBytes,
                                         size_t      &numBlocks) noexcept;

            void *acquire();
            void  release(void *blockAddr) noexcept;


            Chunk        *acquiring; //!< acquiring chunk
            Chunk        *releasing; //!< releasing chunk
            Chunk        *wandering; //!< last empty
            size_t        available; //!< available blocks
            const size_t  blockSize; //!< common blockSize
            const size_t  numBlocks; //!< numBlocks per Chunk
            Pages &       dataPages; //!< the chosen Pages for Chunk I/O
            const size_t  addBlocks; //!< numBlocks-1
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Arena);
            Chunk *queryChunk();
        };

    }

}

#endif

