
#include "y/jive/pattern/basic/single.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Single:: ~Single() noexcept
        {
        }

        
        Single:: Single(const uint8_t c) noexcept :
        Pattern(UUID), code(c)
        {
            Y_PATTERN(Single);
        }

        Pattern * Single:: clone() const
        {
            return new Single(code);
        }
    }

}
