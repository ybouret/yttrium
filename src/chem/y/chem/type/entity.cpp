#include "y/chem/type/entity.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Chemical
    {
        void Entity:: zindx() noexcept
        {
            memset( (void *)&indx[0], 0, sizeof(indx) );
        }

        Entity:: ~Entity() noexcept {}

        Entity:: Entity(const Entity &other) :
        Object(), 
        Counted(),
        Vizible(),
        name(other.name),
        indx()
        {
            memcpy( (void *)&indx[0], &other.indx[0], sizeof(indx) );
        }

        const String & Entity:: key() const noexcept
        {
            return name;
        }


    }

}

