//! \file

#ifndef Y_IO_Libc_IOBuff_Included
#define Y_IO_Libc_IOBuff_Included 1

#include "y/io/chars.hpp"
#include "y/io/stock.hpp"

namespace Yttrium
{
    
    namespace Libc
    {
        class IOBuffer : public IO::Chars
        {
        public:
            explicit IOBuffer();
            virtual ~IOBuffer() noexcept;
            void     ready();           //!< stock.size>=bytes
            void     prune() noexcept;  //!< size+stock.size == bytes
            void     unget(const char); //!< force new char

            const size_t   bytes;
            const unsigned shift;
            char * const   entry;
            IO::Stock      stock;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(IOBuffer);
            void release() noexcept;
        };
    }

}


#endif

