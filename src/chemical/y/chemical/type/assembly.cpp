
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

        Assembly:: Assembly(const Assembly &_) noexcept : maxNameLength(_.maxNameLength) {}

        Assembly & Assembly:: operator=(const Assembly &_) noexcept
        {
            Coerce(maxNameLength) = _.maxNameLength;
            return *this;
        }

        void Assembly:: enroll(const Entity &entity) noexcept
        {
            InSituMax( Coerce(maxNameLength), entity.name->size() );
        }
    }

}


