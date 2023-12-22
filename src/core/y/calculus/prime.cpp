#include "y/calculus/prime.hpp"

namespace Yttrium
{

    namespace Core
    {
        const uint8_t Prime::P8
#include "p8.hxx"
        ;

        const uint16_t Prime::P16
#include "p16.hxx"
        ;
    }

    bool Prime64:: Is_(const uint64_t n) noexcept
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
        while(!Is_(p)) p += 2;
        return p;
    }


}
