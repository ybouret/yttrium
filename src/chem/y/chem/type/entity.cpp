

#include "y/chem/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Entity:: ~Entity() noexcept {}

        Entity:: Entity(const Entity &other) :
        Object(), Counted(), name(other.name) {}
        
        
    }

}

