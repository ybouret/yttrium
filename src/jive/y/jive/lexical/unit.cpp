
#include "y/jive/lexical/unit.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Lexical
        {
            Unit:: ~Unit() noexcept {}

            Unit:: Unit(const Entity &ent,
                        const Context &ctx) noexcept :
            Entity(ent),
            Context(ctx),
            Token(),
            next(0),
            prev(0)
            {
            }
            

            Unit:: Unit(const Unit &other)  :
            Entity( static_cast<const Entity&>(other) ),
            Context(other),
            Token(other),
            next(0),
            prev(0)
            {
            }
        }
        


    }

}

