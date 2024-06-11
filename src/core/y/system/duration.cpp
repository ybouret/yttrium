
#include "y/system/duration.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{
    Duration:: ~Duration() noexcept
    {
        ldz();
    }

    Duration:: Duration() noexcept : wksp()
    {
        ldz();
    }

    void Duration:: ldz() noexcept
    {
        Y_STATIC_ZARR(wksp);
    }

    struct timeval * Duration:: tv() noexcept
    {
        return static_cast<struct timeval *>( Memory::OutOfReach::Addr(wksp) );
    }

}
