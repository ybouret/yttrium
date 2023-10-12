
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

        Entities:: Entities(const Entities &other) noexcept : maxNameSize(other.maxNameSize)
        {
        }
        
        void Entities:: updateWith(const size_t n) noexcept
        {
            Coerce(maxNameSize) = Max(maxNameSize,n);
        }


        void Entities:: updateWith(const Entity &ent) noexcept
        {
            updateWith(ent.name.size());
        }



    }

}


