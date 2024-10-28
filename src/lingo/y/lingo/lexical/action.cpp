#include "y/lingo/lexical/action.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {
            Action_:: ~Action_() noexcept
            {
            }

            Action_:: Action_(const uint32_t t, const bool f) noexcept :
            Object(),
            Counted(),
            uuid(t),
            endl(f),
            self(0)
            {
                
            }
        }

    }

}


