
#include "y/lingo/lexical/unit.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {
            Unit:: ~Unit() noexcept {}

            Unit:: Unit(const Entity  &_root,
                        const Context &_cntx) noexcept :
            Object(),
            Entity(_root),
            Token(),
            next(0),
            prev(0),
            info(_cntx)
            {
            }

            std::ostream & operator<<(std::ostream &os, const Unit &unit)
            {
                os << unit.name << '=' << '\'' << unit.toPrintable() << '\'';
                return os;
            }
        }
        
    }

}
