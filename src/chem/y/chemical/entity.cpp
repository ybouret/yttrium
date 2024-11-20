
#include "y/chemical/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Entity:: ~Entity() noexcept
        {
        }

        const String & Entity:: key() const noexcept
        {
            return name;
        }

    }

}

