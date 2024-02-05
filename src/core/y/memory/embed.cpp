#include "y/memory/embed.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/allocator.hpp"

#include <iostream>

namespace Yttrium
{
    namespace Memory
    {

        size_t Embed:: Align(const size_t n) noexcept
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
            *handle = 0;
            return offset + length;
        }

        void Embed:: link(void *base) noexcept
        {
            assert(0!=base);
            assert(0!=handle);
            *handle = static_cast<void *>( static_cast<char*>(base)+offset );
        }

        void  *Embed:: Build(Embed          embed[],
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
            void *entry = alloc.acquire(bytes,1);
            for(size_t i=0;i<count;++i)
            {
                embed[i].link(entry);
            }
            return entry;
        }

        void * Embed:: address() noexcept
        {
            assert(0!=handle);
            void *addr = *handle; assert(0!=addr);
            return addr;
        }




        std::ostream & operator<<(std::ostream &os, const Embed &emb)
        {
            os << "@" << *emb.handle << " #blocks=" << emb.blocks << " #bytes=" << emb.length;
            return os;
        }
        
    }

}
