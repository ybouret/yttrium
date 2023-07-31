
#include "y/io/stream/input.hpp"
#include "y/system/exception.hpp"

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

    static const char fmt[] ="missing byte#%u from %s";


    template <typename T>
    static inline
    void fetchInit(InputStream &os, T &x, const char *ctx)
    {
        char c = 0;
        if(!os.query(c)) throw Specific::Exception( os.callSign(),fmt,1, os.From(ctx));
        x = uint8_t(c);
    }

    template <typename T>
    static inline
    void fetchNext(InputStream &os, T &x, unsigned i, const char *ctx)
    {
        char c = 0;
        if(!os.query(c)) throw Specific::Exception( os.callSign(),fmt,i, os.From(ctx));
        x <<= 8;
        x |= uint8_t(c);
    }


    bool InputStream:: fetch(uint8_t  &x, const char *ctx)
    {
        fetchInit(*this,x,ctx);
        return true;
    }


    bool InputStream:: fetch(uint16_t  &x, const char *ctx)
    {
        fetchInit(*this,x,ctx);
        fetchNext(*this,x,2,ctx);
        return true;
    }

    bool InputStream:: fetch(uint32_t  &x, const char *ctx)
    {
        fetchInit(*this,x,ctx);
        fetchNext(*this,x,2,ctx);
        fetchNext(*this,x,3,ctx);
        fetchNext(*this,x,4,ctx);
        return true;
    }

    bool InputStream:: fetch(uint64_t  &x, const char *ctx)
    {
        fetchInit(*this,x,ctx);
        fetchNext(*this,x,2,ctx);
        fetchNext(*this,x,3,ctx);
        fetchNext(*this,x,4,ctx);
        fetchNext(*this,x,5,ctx);
        fetchNext(*this,x,6,ctx);
        fetchNext(*this,x,7,ctx);
        fetchNext(*this,x,8,ctx);
        return true;
    }



}


