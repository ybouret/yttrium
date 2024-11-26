
#include "y/chemical/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Entity:: ~Entity() noexcept
        {
        }

        Entity:: Entity(const size_t topLevelIndex) noexcept :
        Object(),
        Counted(),
        indx()
        {
            assert(topLevelIndex>0);
            for(size_t i=0;i<Levels;++i) Coerce(indx[i]) = topLevelIndex;
        }
    }

}
