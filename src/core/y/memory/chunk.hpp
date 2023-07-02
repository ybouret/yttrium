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

            static const char * const CallSign;

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

            bool          owns(const void *blockAddr, const size_t blockSize)   const noexcept; //!< check exactly owned
            Ownership     whose(const void *blockAddr)                          const noexcept; //!< locate address
            void     *    acquire(const size_t blockSize)                             noexcept; //!< acquire zeroed block[block_size]
            bool          release(void *blockAddr, const size_t blockSize)            noexcept; //!< release a previously acquire block
            bool          entails(const void *blockAddr)                        const noexcept;
            static Chunk *MakeFor(const size_t blockSize,
                                  void        *page,
                                  const size_t size) noexcept;

            void displayInfo(const size_t indent, const ptrdiff_t delta=0) const;

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________

            //! sizeof(Chunk) + Y_MEMALIGN(numBlocks*blockSize*);
            static size_t  GetFlatBytes(const size_t blockSize, const uint8_t numBlocks) noexcept;

            //! 2^( Log2(GetFlatBytes(blockSize,255))
            static size_t  OptPageBytes(const size_t blockSize) noexcept;

            //! Min(255,(pageBytes-sizeof(Chunk))/blockSize))
            static uint8_t OptNumBlocks(const size_t blockSize, const size_t pageBytes) noexcept;

            //! Min(255,chunkSize/blockSize)
            static uint8_t GetNumBlocks(const size_t blockSize, const size_t chunkSize) noexcept;
            
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
