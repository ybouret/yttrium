
#include "y/chemical/plexus/boundaries.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Boundaries:: Boundaries(const BBank &bbank) noexcept :
        Proxy<const BList>(),
        list(bbank)
        {
        }

        Boundaries:: ~Boundaries() noexcept
        {
        }
        
    }

}

