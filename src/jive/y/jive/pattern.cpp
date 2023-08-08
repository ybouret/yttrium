
#include "y/jive/pattern.hpp"

namespace Yttrium
{
    namespace Jive
    {

        Pattern:: ~Pattern() noexcept
        {
        }

        Pattern:: Pattern(const uint32_t t) noexcept :
        Object(),
        Token(),
        next(0),
        prev(0),
        self(0),
        uuid(t)
        {
        }

        Pattern:: Pattern(const Pattern &p) noexcept :
        Object(),
        Token(),
        next(0),
        prev(0),
        self(0),
        uuid(p.uuid)
        {

        }
        
    }

}

