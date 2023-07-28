//! \file

#ifndef Y_IO_Libc_IOBuff_Included
#define Y_IO_Libc_IOBuff_Included 1

#include "y/io/chars.hpp"

namespace Yttrium
{
    
    namespace Libc
    {
        class IOBuffer
        {
        public:
            explicit IOBuffer();
            virtual ~IOBuffer() noexcept;

            const size_t   bytes;
            const unsigned shift;
            char * const   chars;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(IOBuffer);
            void release() noexcept;
        };
    }

}


#endif

