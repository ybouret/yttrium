#include "y/memory/embed.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/allocator.hpp"

#include <iostream>
#include <iomanip>
#include "y/text/human-readable.hpp"

namespace Yttrium
{
    namespace Memory
    {

        size_t Embed:: AlignBytes(const size_t n) noexcept
        {
            return Y_MEMALIGN(n);
        }

        Embed:: Embed(const Embed &other) noexcept :
        handle(other.handle),
        offset(other.offset),
        length(other.length),
        blocks(other.blocks)
        {
            assert(0!=handle);
        }
        
        Embed:: ~Embed() noexcept
        {
            assert(0!=handle);
            *handle = 0;
        }

        size_t Embed:: nextOffset()   noexcept
        {
            assert(0!=handle);
            assert(0==*handle);
            return offset + length;
        }

        void Embed:: linkOffset(void *base) noexcept
        {
            assert(0!=base);
            assert(0!=handle);
            assert(0==*handle);
            *handle = static_cast<void *>( static_cast<char*>(base)+offset );
        }

        void  *Embed:: Build(Embed * const  embed,
                             const size_t   count,
                             Allocator   &  alloc,
                             size_t      &  bytes)
        {
            assert(count>0);
            assert(0!=embed);

            //------------------------------------------------------------------
            // format
            //------------------------------------------------------------------
            embed[0].offset = 0;
            for(size_t i=1;i<count;++i)
            {
                embed[i].offset = embed[i-1].nextOffset();
            }
            bytes = embed[count-1].nextOffset();

            //------------------------------------------------------------------
            // acquire
            //------------------------------------------------------------------
            void *base = alloc.acquire(bytes,1);
            for(size_t i=0;i<count;++i)
                embed[i].linkOffset(base);
            return base;
        }

        void * Embed:: address() noexcept
        {
            assert(0!=  handle);
            assert(0!= *handle);
            return *handle;
        }




        std::ostream & operator<<(std::ostream &os, const Embed &emb)
        {
            os << HumanReadable(emb.blocks);
            os << "@" << *emb.handle;
            os << " (" << HumanReadable(emb.length) << "b)";
            return os;
        }
        
    }

}
