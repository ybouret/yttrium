
//! \file

#ifndef Y_Memory_Legacy_Included
#define Y_Memory_Legacy_Included 1

#include "y/memory/allocator.hpp"

namespace Yttrium
{
    class Lockable;

    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //
        //! C Legacy protected memory
        //
        //
        //______________________________________________________________________
        class Legacy
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Legacy();          //!< setup by fetching giant lock
            virtual ~Legacy() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            void *          acquire(const size_t blockSize);                           //!< calloc(1,blockSize), update memory
            void            release(void *blockAddr, const size_t blockSize) noexcept; //!< free(blockAddr), update memory
            static uint64_t Allocated()                                      noexcept; //!< global memory count

            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Legacy);
            Lockable &giant;
        };

    }

}

#endif

