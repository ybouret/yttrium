
#include "y/chemical/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Entity:: ~Entity() noexcept
        {
        }

        Entity:: Entity() noexcept : Oversized()
        {
        }
        

      

        std::ostream & operator<<( std::ostream &os, const Entity &entity)
        {
            return os << entity.key();
        }
    }

}
