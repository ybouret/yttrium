
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


    size_t InputStream:: fetch(uint8_t  &x)
    {
        return fetchInit(*this,x) ? 1 : 0;
    }


    size_t InputStream:: fetch(uint16_t  &x)
    {
        if(!fetchInit(*this,x)) return 0;
        if(!fetchNext(*this,x)) return 1;
        return 2;
    }

    size_t InputStream:: fetch(uint32_t  &x)
    {
        if(!fetchInit(*this,x)) return 0;
        if(!fetchNext(*this,x)) return 1;
        if(!fetchNext(*this,x)) return 2;
        if(!fetchNext(*this,x)) return 3;
        return 4;
    }

    size_t InputStream:: fetch(uint64_t  &x)
    {
        if(!fetchInit(*this,x)) return 0;
        if(!fetchNext(*this,x)) return 1;
        if(!fetchNext(*this,x)) return 2;
        if(!fetchNext(*this,x)) return 3;
        if(!fetchNext(*this,x)) return 4;
        if(!fetchNext(*this,x)) return 5;
        if(!fetchNext(*this,x)) return 6;
        if(!fetchNext(*this,x)) return 7;
        return 8;
    }



}


