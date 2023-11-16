#include "y/concurrent/context.hpp"
#include "y/calculus/base10.hpp"

#include <cstring>
#include <cstdio>

namespace Yttrium
{

    namespace Concurrent
    {

        static inline size_t FormatContext(const char  *buf,
                                           size_t       len,
                                           const size_t size, 
                                           const size_t rank) noexcept
        {
            char                *tgt = (char *)buf; memset(tgt,0,len);
            const Base10::Format fmt  = size;
            snprintf(tgt,len,*fmt,size);
            strcat(tgt,".");
            const size_t shift = strlen(buf);
            len -= shift;
            tgt += shift;
            snprintf(tgt,len,*fmt,rank);
            return strlen(buf);
        }

        Context:: Context(const size_t sz, const size_t rk) noexcept :
        Memory::ReadOnlyBuffer(),
        size(sz),
        rank(rk),
        indx(rank+1),
        name(),
        len_( FormatContext(name,sizeof(name),size,rank) )
        {
            assert(size>0);
            assert(rank<size);
            assert(len_ == strlen(name) );
        }

        Context:: Context() noexcept :
        Memory::ReadOnlyBuffer(),
        size(1),
        rank(0),
        indx(1),
        name(),
        len_( FormatContext(name,sizeof(name),size,rank) )
        {
            assert(len_ == strlen(name) );
        }


        Context:: Context(const Context &other) noexcept :
        Memory::ReadOnlyBuffer(),
        size(other.size),
        rank(other.rank),
        indx(other.indx),
        name(),
        len_(other.len_)
        {
            memcpy( (void*)name, other.name, sizeof(name) );
        }


        Context:: ~Context() noexcept
        {
        }


        const void * Context:: ro_addr() const noexcept
        {
            return name;
        }

        size_t Context:: measure() const noexcept
        {
            return len_;
        }
    }

}

