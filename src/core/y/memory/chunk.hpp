//! \file

#ifndef Y_Memory_Chunk_Included
#define Y_Memory_Chunk_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Memory
    {

        class Chunk
        {
        public:

            enum Ownership
            {
                IsOwnedByPrev,
                IsOwnedByThis,
                IsOwnedByNext
            };

            Chunk(const size_t   block_size,
                  void          *chunk_data,
                  const size_t   chunk_size) noexcept;
            ~Chunk() noexcept;

            bool       owns(const void *addr, const size_t block_size) const noexcept;
            Ownership  whose(const void *addr) const noexcept;
            void     * acquire(const size_t block_size) noexcept;
            bool       release(void *block_addr, const size_t block_size) noexcept;


            uint8_t                first_available; //!< bookkeeping
            uint8_t                still_available; //!< bookkeeping
            uint8_t                operated_number; //!< bookkeeping
            const uint8_t          provided_number; //!< initial count : still_available + operated_number
            uint8_t       *        data;            //!< first item
            const uint8_t * const  last;            //!< first invalid item
            Chunk *                next;
            Chunk *                prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Chunk);
        };

    }

}

#endif
