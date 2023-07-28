
//! \file

#ifndef Y_IO_Libc_Direct_Buffer_Included
#define Y_IO_Libc_Direct_Buffer_Included 1

#include "y/io/libc/buffer/system.hpp"

namespace Yttrium
{

    namespace Libc
    {

        class DirectBuffer : public SystemBuffer
        {
        public:
            explicit DirectBuffer();
            virtual ~DirectBuffer() noexcept;

            char       *       curr;
            const char * const last;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(DirectBuffer);
        };
    }

}


#endif
