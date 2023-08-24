
#include "y/jive/lexical/unit.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Lexical
        {
            Unit:: ~Unit() noexcept {}

            Unit:: Unit(const Context &ctx) noexcept :
            Object(),
            Context(ctx),
            Token(),
            next(0),
            prev(0)
            {
            }

            Unit:: Unit(const Unit &other)  :
            Object(),
            Context(other),
            Token(other),
            next(0),
            prev(0)
            {
            }
        }
        


    }

}

