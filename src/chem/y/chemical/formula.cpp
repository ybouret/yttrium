
#include "y/chemical/formula.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Formula:: ~Formula() noexcept
        {

        }

        Formula:: Formula(XNode * const node) noexcept :
        code(node)
        {
            assert(!code.isEmpty());
        }

    }

}


