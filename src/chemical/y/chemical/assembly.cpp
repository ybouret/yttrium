
#include "y/chemical/assembly.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Assembly:: ~Assembly() noexcept
        {
        }

        Assembly:: Assembly() noexcept : maxLen(0)
        {

        }

        void Assembly:: enroll(const Entity &en) noexcept
        {
            Coerce(maxLen) = Max(maxLen,en.name.size());
        }
    }

}

