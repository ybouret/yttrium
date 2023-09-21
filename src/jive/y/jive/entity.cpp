
#include "y/jive/entity.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Entity:: ~Entity() noexcept
        {
        }

        const String & Entity:: key() const noexcept
        {
            return *name;
        }
    }

}
