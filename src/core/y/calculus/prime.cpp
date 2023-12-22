#include "y/calculus/prime.hpp"

namespace Yttrium
{

    bool Prime64:: Check(const uint64_t n) noexcept
    {
        return Core::Prime::Check<uint64_t>(n);
    }

    uint64_t Prime64::Next(uint64_t p) noexcept
    {
        switch(p)
        {
            case 0:
            case 1:
            case 2:
                return 2;
            default:
                p|=1;
                break;
        }
        while(!Check(p)) p += 2;
        return p;
    }


}
