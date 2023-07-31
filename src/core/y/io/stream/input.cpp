
#include "y/io/stream/input.hpp"
#include "y/system/exception.hpp"
#include "y/text/plural.hpp"

namespace Yttrium
{

    InputStream:: ~InputStream() noexcept
    {
    }

    InputStream:: InputStream() noexcept : Stream() {}
    

    const char *InputStream:: From(const char *ctx) noexcept
    {
        return ctx ? ctx : Core::Unknown;
    }


    size_t InputStream:: query(void *blockAddr, const size_t blockSize)
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
        return query(&x,1);
    }


    size_t InputStream:: fetch(uint16_t  &x)
    {
        uint8_t u[2] = { 0,0 };
        const size_t  nr = query(u,2);
        if(2==nr)
        {
            x = uint16_t(u[0]) + (uint16_t(u[1]) << 8);
        }
        return nr;

    }

    size_t InputStream:: fetch(uint32_t  &x)
    {
        uint8_t       u[4] = { 0,0,0,0 };
        const size_t  nr   = query(u,4);
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
        const size_t  nr   = query(u,8);
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



}

#include "y/io/stream/pack64.hpp"
#include "y/calculus/bit-count.hpp"

namespace Yttrium
{
    uint64_t InputStream:: read64(const char *ctx)
    {
        return IO::Pack64::Read(*this,ctx);

    }

}
