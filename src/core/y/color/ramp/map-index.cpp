
#include "y/color/ramp/map-index.hpp"

namespace Yttrium
{
    namespace Color
    {
        MapIndex:: ~MapIndex() noexcept {}
        MapIndex::  MapIndex(const RampColor * const entry,
                             const size_t            count) noexcept :
        head(entry),
        size(count),
        smod(size-1)
        {
            assert(0!=head);
            assert(size>1);
        }

        RampColor MapIndex:: operator()(const size_t indx) const noexcept
        {
            return head[ indx <= 0 ? 0 : 1 + (indx%smod) ];
        }

    }

}


