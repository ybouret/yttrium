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

        //______________________________________________________________________
        //
        //
        //
        //! list of Chunks of same blockSize
        //
        //
        //______________________________________________________________________
        class Arena : public ListOf<Chunk>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const uint8_t      MinBlocksPerChunk = 16;  //!< alias for consistency
            static const uint8_t      MaxBlocksPerChunk = 255; //!< alias for computations
            static const char * const CallSign;                //!< "Memory::Arena"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! setup by choosing the best Pages in Album with ComputeShift
            /**
             \param userBlockSize blockSize>0
             \param userDataPages persistents pages
             \param userPageBytes target pageBytes
             */
            //__________________________________________________________________
            explicit Arena(const size_t  userBlockSize,
                           Album        &userDataPages,
                           const size_t  userPageBytes);


            virtual ~Arena() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! compute the optimal power of two page size
            /**
             - optPageBytes >= sizeof(Chunk) + Y_MEMALIGN(blockSize*MinBlocksPerChunk)
             - optPageBytes <= Log2(sizeof(Chunk)+ Y_MEMALIGN(blockSize*MaxBlocksPerChunk) <= pageBytes
             \param blockSize the Arena  blockSize
             \param pageBytes the target pageBytes
             \param numBlocks the number of blocks perChunk
             \return the power of two page size
             */
            //__________________________________________________________________
            static unsigned ComputeShift(const size_t blockSize,
                                         const size_t pageBytes,
                                         size_t      &numBlocks) noexcept;


            //__________________________________________________________________
            //
            //! acquire a new block of zeroed memory
            /**
             - uses cache
             - then uses 'available' to trigger of LOCAL look-up
             - or create a new chunk and return its first block
             */
            //__________________________________________________________________
            void *acquire();


            //__________________________________________________________________
            //
            //! release a previously acquired block
            /**
             release a block with some hysterisis:
             - I keep track of empty chunks
             - If TWO are empty, then the highest memory is returned to its page
             \param blockAddr a block a blockSize belonging to this arena
             */
            //__________________________________________________________________
            void  release(void *blockAddr) noexcept;


            //! display statistics
            void displayInfo(const size_t indent) const;

            
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Arena        *next;      //!< for list
            Arena        *prev;      //!< for list
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

