//! \file

#ifndef Y_IO_Libc_System_Buffer_Included
#define Y_IO_Libc_System_Buffer_Included 1

#include "y/io/chars.hpp"
#include "y/io/stock.hpp"

namespace Yttrium
{

    namespace Libc
    {
        
        class SystemBuffer
        {
        public:
            explicit SystemBuffer();
            virtual ~SystemBuffer() noexcept;

            const size_t   bytes;
            const unsigned shift;
            char * const   entry;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(SystemBuffer);
        };
    }

}


#endif

