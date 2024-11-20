
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


        void Entity:: setup(const size_t _indx) noexcept
        {
            for(size_t i=0;i<LEVELS;++i) Coerce(indx[i]) = _indx;
        }

        std::ostream & operator<<(std::ostream &os, const Entity &entity)
        {
            return os << entity.name;
        }

    }

}

