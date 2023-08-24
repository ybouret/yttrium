
#include "y/jive/lexical/unit.hpp"

namespace Yttrium
{
    namespace Jive
    {

        Unit:: ~Unit() noexcept {}

        Unit:: Unit(const Context &ctx) noexcept :
        Context(ctx),
        Token(),
        next(0),
        prev(0)
        {
        }

        Unit:: Unit(const Unit &other)  :
        Context(other),
        Token(other),
        next(0),
        prev(0)
        {
        }



    }

}

