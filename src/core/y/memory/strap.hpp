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
            static const size_t   MinBlockCount = 4;                           //!< strap+head+data+tail
            static const unsigned MinBlockShift = iLog2<MinBlockCount>::Value; //!< helper for shift__()
            static const size_t   MinBytes;                                    //!< MinBlockCount * sizeof(Strap)


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
            static Strap * Release(void *  blockAddr) noexcept;
            size_t         shift__() const noexcept;

            static size_t BlockSizeFor(const size_t blockSize) noexcept;
            static size_t ShiftToHold(const size_t blockSize);


            void   displayInfo(size_t indent) const;

            Strap *next;
            Strap *prev;
            Block *head;
            Block *tail;



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Strap);
        };

    }

}

#endif

