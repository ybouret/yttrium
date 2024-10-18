#include "y/lingo/pattern/logic/and.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        And:: And() noexcept :
        Logic(UUID)
        {
        }

        And:: ~And() noexcept {}

        And:: And(const And &other) : Logic(other) {}

    }

}


