
#include "y/io/stream/output.hpp"

namespace Yttrium
{
    OutputStream:: ~OutputStream() noexcept {}

    OutputStream:: OutputStream() noexcept {}

    void OutputStream:: write(const void *blockAddr, const size_t blockSize)
    {
        assert( Good(blockAddr,blockSize) );
        const char *C = static_cast<const char *>(blockAddr);
        size_t      n = blockSize;
        while(n-- > 0) write( *(C++) );
    }

    OutputStream & OutputStream:: operator<<(const char c)
    {
        write(c);
        return *this;
    }

    OutputStream & OutputStream:: operator<<(const char *C)
    {
        if(0!=C)
        {
            while( 0 != *C ) write(*(C++));
        }
        return *this;
    }

    OutputStream & OutputStream:: operator<<(const Memory::ReadOnlyBuffer &buffer)
    {
        write(buffer.ro_addr(),buffer.measure());
        return *this;
    }


}


