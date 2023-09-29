
#include "y/chem/type/entities.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Entities:: ~Entities() noexcept
        {
        }

        Entities:: Entities() noexcept : maxNameSize(0)
        {
        }

        void Entities:: updateWith(const Entity &ent) noexcept
        {
            Coerce(maxNameSize) = Max(maxNameSize,ent.name.size());
        }



    }

}


