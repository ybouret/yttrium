#include "y/field/layout/3d.hpp"

namespace Yttrium
{

    namespace Field
    {

        Layout3D:: ~Layout3D() noexcept
        {
            Coerce(nr) = 0;
        }

        Layout3D:: Layout3D(const Coord3D lo,
                            const Coord3D up) noexcept :
        Layout3D_(lo,up),
        nr(width.y*width.z)
        {
        }

        size_t Layout3D:: numRows() const noexcept
        {
            return nr;
        }

        size_t Layout3D:: numSlices() const noexcept
        {
            return width.z;
        }
    }

}


