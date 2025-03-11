
#include "y/chemical/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Entity:: ~Entity() noexcept
        {
        }

        
        const String & Entity:: key() const noexcept
        {
            return *name;
        }


        Entity:: Entity(const String *_) noexcept : name(_) {}
        Entity:: Entity(const XName & _) noexcept : name(_) {}
    }

}

