
#include "y/ink/slab.hpp"

namespace Yttrium
{
    namespace Ink
    {
        Slab:: Slab(const ThreadContext &ctx) noexcept :
        Concurrent::Frame2D<unit_t>(ctx),
        hseg( *(const HSegment **) Memory::OutOfReach::Addr(&segment) )
        {
        }

        Slab:: ~Slab() noexcept
        {
            
        }
    }

}
