//! \file

#ifndef Y_Memory_Strap_Included
#define Y_Memory_Strap_Included 1

#include "y/calculus/ilog2.hpp"

namespace Yttrium
{

    namespace Memory
    {

        struct Block;

        //______________________________________________________________________
        //
        //
        //
        //! Strap of internal Blocks
        //
        //
        //______________________________________________________________________
        class Strap
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign;                                    //!< "Memory::Strap"
            static const size_t       MinBlockCount = 4;                           //!< strap+head+data+tail
            static const unsigned     MinBlockShift = iLog2<MinBlockCount>::Value; //!< helper for shift__()
            static const size_t       MinBytes;                                    //!< MinBlockCount * sizeof(Strap)


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Strap(void *addr, const size_t size) noexcept; //!< setup a addr[size=power of two]
            ~Strap()                             noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! acquire the tightest possible block >= blockSize
            /**
             - upon success, update blockSize
             - upon failure, return 0, blockSize is left untouched
             */
            //__________________________________________________________________
            void *         acquire(size_t &blockSize) noexcept;

            //__________________________________________________________________
            //
            //! release a previously acquired block
            /**
             \param blockAddr a valid block address
             \return owning Strap
             */
            //__________________________________________________________________
            static Strap * Release(void *  blockAddr) noexcept;

            //__________________________________________________________________
            //
            //! check if fully vacant
            //__________________________________________________________________
            bool           isEmpty() const noexcept;

            //__________________________________________________________________
            //
            //! compute original memory bytes = 2^myShift
            //__________________________________________________________________
            unsigned       myShift() const noexcept;

            //__________________________________________________________________
            //
            //! return positive, aligned block size
            //__________________________________________________________________
            static size_t BlockSizeFor(const size_t blockSize) noexcept;


            //__________________________________________________________________
            //
            //! 2^ShiftToHold is the page size required to hold blockSize
            //__________________________________________________________________
            static unsigned ShiftToHold(const size_t blockSize);

            //__________________________________________________________________
            //
            //! display statistics
            //__________________________________________________________________
            void   displayInfo(size_t indent) const;

            //! detect anonymous ownership
            bool owns(const void *blockAddr) const noexcept;

            //! probe for  size of an OWNED block
            size_t blockSizeOf(const void *blockAddr) const noexcept;

            //! from head to blockAddr that must be OWNED
            const void *prevAddr(const void *blockAddr, size_t &prevSize) const noexcept;

            //! from next block to tail include, blockAddr must be OWNED
            const void *nextAddr(const void *blockAddr, size_t &nextSize) const noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Strap *next; //!< for list
            Strap *prev; //!< for list
            Block *head; //!< sentinel for internal linked list
            Block *tail; //!< sentinel for internal linked list



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Strap);
        };

    }

}

#endif

