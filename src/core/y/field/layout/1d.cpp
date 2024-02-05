
#include "y/field/layout/1d.hpp"

namespace Yttrium
{

    namespace Field
    {
        
        Layout1D:: ~Layout1D() noexcept
        {
        }

        Layout1D:: Layout1D(const Coord1D lo,
                            const Coord1D up) noexcept :
        Layout1D_(lo,up)
        {
        }
        
    }

}


