
#include "y/container/cxx-capacity.hpp"


namespace Yttrium
{
    size_t CxxSetCapacity<CxxRequiredCapacity> :: From(const size_t n, const Memory::Crux::Wad &) noexcept
    {
        return n;
    }

    size_t CxxSetCapacity<CxxAcquiredCapacity> :: From(const size_t, const Memory::Crux::Wad &w) noexcept
    {
        return w.maxBlocks;
    }

}

