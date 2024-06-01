
#include "y/kemp/natural.hpp"
#include "y/kemp/element.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        SignType Natural:: Compare(const Natural &lhs, const Natural &rhs) noexcept
        {
            return Element::Compare(*lhs,*rhs);
        }

        SignType Natural:: Compare(const uint64_t lhs, const Natural &rhs) noexcept
        {
            return Element::Compare(lhs,*rhs);
        }

        SignType Natural:: Compare(const Natural &lhs, const uint64_t rhs) noexcept
        {
            return Element::Compare(*lhs,rhs);
        }
    }

}
