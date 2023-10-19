
#include "y/chem/reactive/army.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Army:: Army() noexcept : 
        Object(),
        prodOnly(),
        reacOnly(),
        spurious(),
        definite()
        {
        }

        Army:: ~Army() noexcept
        {
        }

    }

}
