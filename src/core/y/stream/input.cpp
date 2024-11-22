
#include "y/stream/input.hpp"
#include "y/system/exception.hpp"
#include "y/text/plural.hpp"

namespace Yttrium
{

    InputStream:: ~InputStream() noexcept
    {
    }

    InputStream:: InputStream() noexcept : Stream() {}
    

    const char *InputStream:: From(const char * const ctx) noexcept
    {
        return 0!=ctx ? ctx : "variable";
    }


    size_t InputStream:: fetch(void *blockAddr, const size_t blockSize)
    {
        assert(Good(blockAddr,blockSize));
        size_t num = 0;
        char  *ptr = static_cast<char *>(blockAddr);
        for(num=0;num<blockSize;++num)
        {
            if(!query(ptr[num])) break;
        }
        return num;
    }



    size_t InputStream:: fetch(uint8_t  &x)
    {
        return fetch(&x,1);
    }


    size_t InputStream:: fetch(uint16_t  &x)
    {
        uint8_t u[2] = { 0,0 };
        const size_t  nr = fetch(u,2);
        if(2==nr)
        {
            x = uint16_t(u[0]) + (uint16_t(u[1]) << 8);
        }
        return nr;

    }

    size_t InputStream:: fetch(uint32_t  &x)
    {
        uint8_t       u[4] = { 0,0,0,0 };
        const size_t  nr   = fetch(u,4);
        if(4==nr)
        {
            x = uint32_t(u[0])
            +  (uint32_t(u[1]) << 8)
            +  (uint32_t(u[2]) << 16)
            +  (uint32_t(u[3]) << 24);
        }
        return nr;
    }

    size_t InputStream:: fetch(uint64_t  &x)
    {
        uint8_t       u[8] = { 0,0,0,0,0,0,0,0 };
        const size_t  nr   = fetch(u,8);
        if(8==nr)
        {
            x = uint64_t(u[0])
            +  (uint64_t(u[1]) << 8)
            +  (uint64_t(u[2]) << 16)
            +  (uint64_t(u[3]) << 24)
            +  (uint64_t(u[4]) << 32)
            +  (uint64_t(u[5]) << 40)
            +  (uint64_t(u[6]) << 48)
            +  (uint64_t(u[7]) << 56);
        }
        return nr;
    }

    void InputStream:: missingBytes(const size_t n, const char *ctx) const
    {
        assert(n>0);
        throw Specific::Exception( callSign(), "missing %u byte%s for %s", unsigned(n), Plural::s(n), From(ctx) );
    }

    void InputStream:: overflowing(const char *ctx) const
    {
        throw Specific::Exception( callSign(), "overflowing read for %s", From(ctx));
    }

    bool InputStream:: gets(IO::Chars &line)
    {
        static const char CR = '\r';
        static const char LF = '\n';

        line.release();
        char first = 0;
        while(query(first))
        {
            switch(first)
            {
                case LF:
                    // single LF -> EOL
                    return true;

                case CR:
                {
                    char second = 0;
                    if(!query(second))
                        return true; // single final CR
                    if(LF==second)
                    {
                        // found CRLF
                        return true;
                    }
                    else
                    {
                        // unread
                        store(second);
                        return true; // it was single CR
                    }
                }


                default:
                    // append to line
                    line << first;
            }
        }

        return line.size>0;
    }

}

#include "y/stream/io/pack64.hpp"
//#include "y/calculus/bit-count.hpp"

namespace Yttrium
{
    uint64_t InputStream:: read64(const char * const ctx)
    {
        return IO::Pack64::Read(*this,ctx);
    }

}

#include "y/string.hpp"
namespace Yttrium
{
    bool InputStream:: gets(Core::String<char> &line)
    {
        IO::Chars temp;
        if( gets(temp) )
        {
            const size_t       n = temp.size;
            Core::String<char> s(n,AsCapacity,true);
            for(size_t i=n;i>0;--i)
            {
                s[i] = temp.pullTail();
            }
            line.swapWith(s);
            return true;
        }
        else
            return false;
    }

}
