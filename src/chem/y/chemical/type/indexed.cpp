
#include "y/chemical/type/indexed.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Indexed:: Indexed(const size_t topLevelIndex) noexcept :
        Entity(),
        indx()
        {
            assert(topLevelIndex>0);
            for(size_t i=0;i<Levels;++i) Coerce(indx[i]) = topLevelIndex;
        }

        Indexed:: ~Indexed() noexcept
        {

        }
    }

}
