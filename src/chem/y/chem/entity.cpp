

#include "y/chem/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Entity:: ~Entity() noexcept {}

        Entity:: Entity(const Entity &other) :
        Object(), Counted(), name(other.name) {}
        
    }

}

