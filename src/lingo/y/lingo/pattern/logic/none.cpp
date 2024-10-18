

#include "y/lingo/pattern/logic/none.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        None:: None() noexcept :
        Logic(UUID)
        {
        }

        None:: ~None() noexcept {}

        None::None(const None &other) : Logic(other) {}

    }

}


