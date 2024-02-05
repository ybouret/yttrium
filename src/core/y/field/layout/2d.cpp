

#include "y/field/layout/2d.hpp"

namespace Yttrium
{

    namespace Field
    {

        Layout2D:: ~Layout2D() noexcept
        {
        }

        Layout2D:: Layout2D(const Coord2D lo,
                            const Coord2D up) noexcept :
        Layout2D_(lo,up)
        {
        }

        size_t Layout2D:: numRows() const noexcept
        {
            return width.y;
        }
    }

}


