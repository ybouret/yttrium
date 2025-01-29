
#include "y/apex/integer.hpp"

namespace Yttrium
{

    namespace Apex
    {
        bool operator==(const Integer &lhs, const Integer &rhs) noexcept
        {
            return (lhs.s == rhs.s) && (lhs.n == rhs.n);
        }

    }

}

namespace Yttrium
{

    namespace Apex
    {
        bool operator!=(const Integer &lhs, const Integer &rhs) noexcept
        {
            return (lhs.s != rhs.s) || (lhs.n == rhs.n);
        }

    }

}
