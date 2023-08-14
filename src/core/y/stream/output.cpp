
#include "y/stream/output.hpp"

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


    void OutputStream :: issue(const uint8_t  &x)
    {
        write(x);
    }

    void OutputStream :: issue(const uint16_t  &x)
    {
        write( uint8_t(x)    );
        write( uint8_t(x>>8) );
    }

    void OutputStream :: issue(const uint32_t  &x)
    {
        write( uint8_t(x)    );
        write( uint8_t(x>>8) );
        write( uint8_t(x>>16) );
        write( uint8_t(x>>24) );
    }

    void OutputStream :: issue(const uint64_t  &x)
    {
        write( uint8_t(x)    );
        write( uint8_t(x>>8) );
        write( uint8_t(x>>16) );
        write( uint8_t(x>>24) );
        write( uint8_t(x>>32) );
        write( uint8_t(x>>40) );
        write( uint8_t(x>>48) );
        write( uint8_t(x>>56) );

    }

}

#include "y/system/exception.hpp"
#include "y/lockable.hpp"
#include "y/memory/buffer/of.hpp"
#include <cstdarg>
#include <cstdio>
#include <cerrno>

namespace Yttrium
{

    OutputStream & OutputStream:: operator()(const char *fmt,...)
    {
        static const char fn[] = "OutputStream";

        assert(fmt!=NULL);
        Y_GIANT_LOCK();
        int res = 0;
        {
            va_list ap;
            va_start(ap,fmt);
            res = vsnprintf(NULL,0,fmt,ap);
            va_end(ap);
            if(res<0) throw Libc::Exception(errno,"%s(...)",fn);
        }
        if(res>0)
        {
            const size_t           blockSize = res;
            const size_t           bufLength = blockSize+1;
            Memory::BufferOf<char> buf(bufLength);
            char                  *blockAddr = &buf[0];
            va_list ap;
            va_start(ap,fmt);
            const int chk = vsnprintf(blockAddr,bufLength,fmt,ap);
            va_end(ap);
            if(res!=chk) throw  Specific::Exception(fn,"lengths mismatch!");
            write(blockAddr,blockSize);
        }

        return *this;
    }

}

#include "y/stream/io/pack64.hpp"


namespace Yttrium
{

    size_t OutputStream:: emit64(const uint64_t u)
    {
        return IO::Pack64::Emit(*this,u);
    }
}
