
#include "y/chemical/type/assembly.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Assembly:: ~Assembly() noexcept
        {
        }

        Assembly:: Assembly() noexcept : maxNameSize(0)
        {
        }

        Assembly:: Assembly(const Assembly &_) noexcept : maxNameSize(_.maxNameSize)
        {
        }

        void Assembly:: enroll(const Entity &entity) noexcept
        {
            const size_t rhs = entity.name.size();
            Coerce(maxNameSize) = Max(maxNameSize,rhs);
        }

    }
}

