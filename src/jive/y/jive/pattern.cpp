
#include "y/jive/pattern.hpp"
#include "y/stream/output.hpp"

namespace Yttrium
{
    namespace Jive
    {

        Pattern:: ~Pattern() noexcept
        {
        }

        Pattern:: Pattern(const uint32_t t) noexcept :
        Object(),
        Counted(),
        Token(),
        Serializable(),
        next(0),
        prev(0),
        self(0),
        uuid(t)
        {
        }

        Pattern:: Pattern(const Pattern &p) noexcept :
        Object(),
        Counted(),
        Token(),
        Serializable(),
        next(0),
        prev(0),
        self(0),
        uuid(p.uuid)
        {

        }

        bool Pattern:: isRegular() const noexcept
        {
            return ! isFragile();
        }

        bool operator==(const Pattern &lhs, const Pattern &rhs) noexcept
        {
            return lhs.isEqualTo(rhs);
        }

        bool operator!=(const Pattern &lhs, const Pattern &rhs) noexcept
        {
            return !lhs.isEqualTo(rhs);
        }

        
    }

}

