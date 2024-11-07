

#include "y/lingo/syntax/internal/compound/alternate.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            Alternate:: ~Alternate() noexcept
            {
            }

            Alternate & Alternate:: operator |= (const Rule &rule)
            {
                add(rule);
                return *this;
            }
        }

    }

}
