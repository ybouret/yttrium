//! \file

#ifndef Y_Memory_RW_Buffer_Included
#define Y_Memory_RW_Buffer_Included 1

#include "y/memory/buffer/ro.hpp"

namespace Yttrium
{
    namespace Memory
    {

        class ReadWriteBuffer : public ReadOnlyBuffer

        {
        protected:
            explicit ReadWriteBuffer() noexcept;
        public:
            virtual ~ReadWriteBuffer() noexcept;

            void *rw_addr() noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ReadWriteBuffer);
        };

    }
}

#endif
