

#include "y/lingo/pattern/logic/none.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        None:: None() noexcept :
        Logic(UUID)
        {
            Y_Lingo_Pattern(None);
        }

        None:: ~None() noexcept {}

        None::None(const None &other) :
        Logic(other)
        {
            Y_Lingo_Pattern(None);
        }

    }

}


