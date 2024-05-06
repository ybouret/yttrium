
#include "y/color/ramp/map-index.hpp"

namespace Yttrium
{
    namespace Color
    {
        MapIndex:: ~MapIndex() noexcept {}
        MapIndex::  MapIndex(const RampColor * const entry,
                             const size_t            count) noexcept :
        head(entry),
        size(count)
        {
            assert(0!=head);
            assert(size>0);
        }

        RampColor MapIndex:: operator()(const size_t indx) const noexcept
        {
            return head[indx%size];
        }

    }

}


