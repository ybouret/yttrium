
#include "y/jive/syntax/rule.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            bool Rule:: Verbose = false;

            Rule:: ~Rule() noexcept {}


            bool Rule:: isTough() const noexcept
            {
                return !isFrail();
            }

        }

    }

}
