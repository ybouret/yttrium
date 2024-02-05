
#include "y/field/layout/4d.hpp"

namespace Yttrium
{

    namespace Field
    {

        Layout4D:: ~Layout4D() noexcept
        {
        }

        Layout4D:: Layout4D(const Coord4D lo,
                            const Coord4D up) noexcept :
        Layout4D_(lo,up),
        ns(width.z*width.w),
        nr(ns*width.y)
        {
        }

        size_t Layout4D:: numVolumes() const noexcept {
            return width.w;
        }

        size_t Layout4D:: numRows() const noexcept
        {
            return nr;
        }

        size_t Layout4D:: numSlices() const noexcept
        {
            return ns;
        }
    }

}
