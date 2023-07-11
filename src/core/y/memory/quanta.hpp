//! \file

#ifndef Y_Memory_Quanta_Included
#define Y_Memory_Quanta_Included 1

#include "y/memory/album.hpp"

namespace Yttrium
{

    namespace Memory
    {
        class Blocks;
        class Straps;

        //______________________________________________________________________
        //
        //
        //
        //! Allocating quanta of Memory
        /**
         - use Blocks for blockSize <= LimitSize
         - use Straps for blockSize >  LimitSize
         */
        //
        //
        //______________________________________________________________________
        class Quanta
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const size_t       LimitSize = 256; //!< behaviour change
            static const char * const CallSign;        //!< "Memory::Quanta"
            
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            explicit Quanta(Blocks &, Straps &) noexcept; //!< initialize from persistent
            virtual ~Quanta()                   noexcept; //!< cleanup


            //__________________________________________________________________
            //
            //
            // interface
            //
            //__________________________________________________________________
            void * acquire(size_t blockSize);                                 //!< call blocks or straps
            void   release(void *blockAddr, const size_t blockSize) noexcept; //!< call blocks or straps

            //! display statistics
            void displayInfo(const size_t indent) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Quanta);
            Blocks &blocks;
            Straps &straps;
        };
    }

}

#endif

