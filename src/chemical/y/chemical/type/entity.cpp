#include "y/chemical/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        
        Entity:: ~Entity() noexcept
        {
            initialize(0);
        }

        const String & Entity:: key() const noexcept
        {
            return name;
        }
        
        void Entity:: initialize(const size_t level) noexcept
        {
            for(size_t i=0;i<Levels;++i)
            {
                Coerce(indx[i]) = level;
            }
        }


    }
}
