
#include "y/lingo/pattern/logic.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Logic:: ~Logic() noexcept
        {
        }

        Logic:: Logic(const uint32_t t) noexcept :
        Pattern(t),
        Pattern::List()
        {
        }

        Logic:: Logic(const Logic &logic) :
        Pattern(logic),
        Pattern::List(logic)
        {
        }
        


    }

}

