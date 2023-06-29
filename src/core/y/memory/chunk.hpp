//! \file

#ifndef Y_Memory_Chunk_Included
#define Y_Memory_Chunk_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Memory
    {

        //______________________________________________________________________
        //
        //
        //! basic chunk of same sized blocks
        //
        //______________________________________________________________________
        class Chunk
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________

            //! Ownership for address lookup
            enum Ownership
            {
                IsOwnedByPrev, //!< address before this
                IsOwnedByThis, //!< address in this
                IsOwnedByNext  //!< addess after this
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            Chunk(const size_t   block_size,
                  void          *chunk_data,
                  const size_t   chunk_size) noexcept;
            ~Chunk() noexcept;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            bool       owns(const void *addr, const size_t block_size) const noexcept;
            Ownership  whose(const void *addr) const noexcept;
            void     * acquire(const size_t block_size) noexcept;
            bool       release(void *block_addr, const size_t block_size) noexcept;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            uint8_t                first_available; //!< bookkeeping
            uint8_t                still_available; //!< bookkeeping
            uint8_t                operated_number; //!< bookkeeping
            const uint8_t          provided_number; //!< initial count : still_available + operated_number
            uint8_t       *        data;            //!< first item
            const uint8_t * const  last;            //!< first invalid item
            Chunk *                next;            //!< for list
            Chunk *                prev;            //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Chunk);
        };

    }

}

#endif
