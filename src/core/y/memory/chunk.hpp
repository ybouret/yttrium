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
            Chunk(const size_t   blockSize,
                  void          *chunkData,
                  const size_t   chunkSize) noexcept;
            ~Chunk() noexcept;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            bool          owns(const void *addr, const size_t block_size)       const noexcept; //!< check exactly owned
            Ownership     whose(const void *addr)                               const noexcept; //!< locate address
            void     *    acquire(const size_t block_size)                            noexcept; //!< acquire zeroed block[block_size]
            bool          release(void *block_addr, const size_t block_size)          noexcept; //!< release a previously acquire block

            static Chunk *Create(const size_t blockSize,
                                 void        *page,
                                 const size_t size) noexcept;

            static size_t  GetFlatBytes(const size_t blockSize, const uint8_t numBlocks) noexcept;
            static size_t  OptPageBytes(const size_t blockSize) noexcept;
            static uint8_t OptNumBlocks(const size_t blockSize, const size_t pageBytes) noexcept;
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            uint8_t                firstAvailable;  //!< bookkeeping
            uint8_t                stillAvailable;  //!< bookkeeping
            uint8_t                operatedNumber;  //!< bookkeeping
            const uint8_t          providedNumber;  //!< initial count : still_available + operated_number
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
