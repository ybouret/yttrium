
#include "y/memory/object-locator.hpp"
#include "y/memory/grains.hpp"
#include <iostream>

#include "y/memory/arena.hpp"
#include "y/memory/strap.hpp"

namespace Yttrium
{
    namespace Memory
    {

        ObjectLocator:: ~ObjectLocator() noexcept
        {
            Coerce(where) = 0;
            Coerce(arena) = 0;
            Coerce(chunk) = 0;
            Coerce(strap) = 0;
        }

        ObjectLocator:: ObjectLocator(const void *blockAddr) :
        where(blockAddr), arena(0), chunk(0), strap(0), width(0)
        {
            if(where)
            {
                const Memory::Grains & grains = Grains::Instance();
                grains.lookFor(*this);

                if(0!=arena)
                    Coerce(width) = arena->blockSize;

                if(0!=strap)
                    Coerce(width) = strap->blockSizeOf(where);

            }
        }

     

        std::ostream & operator<<(std::ostream &os, const ObjectLocator &loc)
        {
            if(0!=loc.arena)
            {
                assert(0!=loc.chunk);
                os << "@Arena[" << loc.width << "]";
                return os;
            }

            if(0!=loc.strap)
            {
                assert(0==loc.chunk);
                os << "@Strap[blockSize=" << loc.width << "]";
                return os;
            }

            os << "@NoObject";

            return os;
        }

        const void * ObjectLocator:: prevAddr(size_t &prevSize) const noexcept
        {
            if(0!=arena)
            {
                assert(0!=chunk);
                const uint8_t *init = chunk->data;
                const uint8_t *here = static_cast<const uint8_t *>(where);
                prevSize = here-init;
                return init;
            }

            if(0!=strap)
                return strap->prevAddr(where,prevSize);
            

            prevSize = 0;
            return 0;
        }

        const void * ObjectLocator:: nextAddr(size_t &nextSize) const noexcept
        {
            if(0!=arena)
            {
                assert(0!=chunk);
                const uint8_t *last = chunk->last;
                const uint8_t *next = static_cast<const uint8_t *>(where)+arena->blockSize;
                nextSize = last-next;
                return next;
            }

            if(0!=strap)
                return strap->nextAddr(where,nextSize);

            nextSize = 0;
            return 0;
        }
    }

}

namespace Yttrium
{
    namespace Memory
    {

        ObjectGuard:: ObjectGuard(const ObjectLocator &objloc, ObjectLocator::Query query) noexcept :
        blockSize(0), blockAddr((objloc.*query)(Coerce(blockSize)))
        {

        }

        ObjectGuard::  ~ObjectGuard() noexcept
        {

        }


    }

}

namespace Yttrium
{
    namespace Memory
    {

        ObjectSentry:: ~ObjectSentry() noexcept {}

        ObjectSentry:: ObjectSentry(const ObjectLocator &objloc, ObjectLocator::Query query) noexcept :
        ObjectGuard(objloc,query),
        Sentry(blockAddr,blockSize)
        {
        }

    }

}

namespace Yttrium
{
    namespace Memory
    {

        ObjectSentries:: ObjectSentries(const void *blockAddr) :
        ObjectLocator(blockAddr),
        prev(*this, & ObjectLocator:: prevAddr),
        next(*this, & ObjectLocator:: nextAddr)
        {

        }

        ObjectSentries:: ~ObjectSentries() noexcept
        {

        }

    }

}


