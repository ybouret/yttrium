
#include "y/chemical/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
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

