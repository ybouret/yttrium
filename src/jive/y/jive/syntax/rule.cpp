
#include "y/jive/syntax/rule.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {
 
            Rule:: ~Rule() noexcept {}


            bool Rule:: isTough() const noexcept
            {
                return !isFrail();
            }

        }

    }

}
