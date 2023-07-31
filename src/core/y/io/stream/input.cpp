
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



    template <typename T>
    static inline
    bool fetchInit(InputStream &os, T &x)
    {
        char c = 0;
        if(!os.query(c)) return false;
        x = uint8_t(c);
        return true;
    }

    template <typename T>
    static inline
    bool fetchNext(InputStream &os, T &x)
    {
        char c = 0;
        if(!os.query(c)) return false;
        x <<= 8;
        x |= uint8_t(c);
        return true;
    }


    bool InputStream:: fetch(uint8_t  &x)
    {
        return fetchInit(*this,x);
    }


    bool InputStream:: fetch(uint16_t  &x)
    {
        if(!fetchInit(*this,x)) return false;
        if(!fetchNext(*this,x)) return false;
        return true;
    }

    bool InputStream:: fetch(uint32_t  &x)
    {
        if(!fetchInit(*this,x)) return false;
        if(!fetchNext(*this,x)) return false;
        if(!fetchNext(*this,x)) return false;
        if(!fetchNext(*this,x)) return false;
        return true;
    }

    bool InputStream:: fetch(uint64_t  &x)
    {
        if(!fetchInit(*this,x)) return false;
        if(!fetchNext(*this,x)) return false;
        if(!fetchNext(*this,x)) return false;
        if(!fetchNext(*this,x)) return false;
        if(!fetchNext(*this,x)) return false;
        if(!fetchNext(*this,x)) return false;
        if(!fetchNext(*this,x)) return false;
        if(!fetchNext(*this,x)) return false;
        return true;
    }



}


