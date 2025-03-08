
#include "y/chemical/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Y_SHALLOW_IMPL(AsName);

        Entity:: ~Entity() noexcept
        {
        }

        Entity:: Entity(const Entity &_) noexcept :
        name(_.name)
        {
        }

        const String & Entity:: key() const noexcept
        {
            return *name;
        }

    }

}

