
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
        where(blockAddr), arena(0), chunk(0), strap(0)
        {
            if(where)
            {
                const Memory::Grains & grains = Grains::Instance();
                grains.lookFor(*this);
            }
        }

        std::ostream & operator<<(std::ostream &os, const ObjectLocator &loc)
        {
            if(0!=loc.arena)
            {
                assert(0!=loc.chunk);
                os << "@Arena[" << loc.arena->blockSize << "]";
                return os;
            }

            if(0!=loc.strap)
            {
                assert(0==loc.chunk);
                os << "@Strap[blockSize=" << loc.strap->blockSizeOf(loc.where) << "]";
                return os;
            }

            os << "@NoObject";

            return os;
        }


    }

}

