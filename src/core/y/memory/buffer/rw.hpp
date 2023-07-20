//! \file

#ifndef Y_Memory_RW_Buffer_Included
#define Y_Memory_RW_Buffer_Included 1

#include "y/memory/buffer/ro.hpp"

namespace Yttrium
{
    namespace Memory
    {

        //______________________________________________________________________
        //
        //
        //
        //! Writable memory buffer
        //
        //
        //______________________________________________________________________
        class ReadWriteBuffer : public ReadOnlyBuffer
        {
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            explicit ReadWriteBuffer() noexcept; //!< setup
        public:
            virtual ~ReadWriteBuffer() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void *rw_addr() noexcept; //!< make writable ro_addr()


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ReadWriteBuffer);
        };

    }
}

#endif
