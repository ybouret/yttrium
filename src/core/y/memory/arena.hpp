
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

        class Arena : public ListOf<Chunk>
        {
        public:
            static const uint8_t MinBlocksPerChunk = 16;
            static const uint8_t MaxBlocksPerChunk = 255;

            explicit Arena(const size_t  userBlockSize,
                           Pages        &userDataPages);
            virtual ~Arena() noexcept;

            static unsigned ComputeShift(const size_t blockSize,
                                         const size_t pageBytes) noexcept;

            const size_t blockSize;
            Pages &      dataPages;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Arena);
        };

    }

}

#endif

