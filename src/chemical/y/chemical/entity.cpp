
#include "y/chemical/entity.hpp"
#include "y/system/exception.hpp"

#include <cstring>

namespace Yttrium
{
    namespace Chemical
    {

        const String & Entity:: key() const noexcept
        {
            return name;
        }

        
        Entity:: ~Entity() noexcept
        {
        }
        

    }

}



