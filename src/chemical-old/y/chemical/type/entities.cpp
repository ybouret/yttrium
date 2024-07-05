#include "y/chemical/type/entities.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Entities:: Entities() noexcept : maxLength(0) {}

        Entities:: ~Entities() noexcept
        {
            Coerce(maxLength)=0;
        }

        void Entities:: updateWith(const Entity &entity) noexcept
        {
            Coerce(maxLength) = Max(maxLength,entity.name.size());
        }

        Entities:: Entities(const Entities &other) noexcept :
        maxLength(other.maxLength)
        {

        }
    }

}


