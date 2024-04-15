#include "y/concurrent/context.hpp"
#include "y/calculus/base10.hpp"

#include <cstring>
#include <cstdio>
#include <iostream>

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

        static inline
        Property FormatProperty(const size_t size, const size_t rank) noexcept
        {
            assert(size>0);
            if(1==size)
            {
                return OnlyOne;
            }
            else
            {
                assert(size>=2);
                if(rank<=0)
                {
                    return Leading;
                }
                else
                {
                    if(rank<size-1)
                    {
                        return Generic;
                    }
                    else
                    {
                        assert(size-1==rank);
                        return Closing;
                    }
                }
            }
        }

        Context:: Context(const size_t sz, const size_t rk) noexcept :
        Memory::ReadOnlyBuffer(),
        size(sz),
        rank(rk),
        indx(rank+1),
        last(size-1),
        ppty( FormatProperty(size,rank) ),
        parallel(size>1),
        primary(rank<=0),
        replica(!primary),
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
        last(0),
        ppty(OnlyOne),
        parallel(false),
        primary(true),
        replica(false),
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
        last(other.last),
        ppty(other.ppty),
        parallel(other.parallel),
        primary(other.primary),
        replica(other.replica),
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

        size_t Context:: next1D() const noexcept
        {
            const size_t res= rank+1;
            return (res >= size) ? 0 : res;
        }

        size_t Context:: prev1D() const noexcept
        {
            return (rank<=0) ? last : rank-1;
        }

        std::ostream & operator<<(std::ostream &os, const Context &ctx)
        {
            os << ctx.name;
            return os;
        }
    }

}

