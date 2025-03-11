
#include "y/chemical/type/assembly.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Assembly:: ~Assembly() noexcept
        {
        }

        Assembly:: Assembly() noexcept : maxNameLength(0)
        {
        }

        void Assembly:: enroll(const Entity &entity) noexcept
        {
            InSituMax( Coerce(maxNameLength), entity.name->size() );
        }
    }

}


