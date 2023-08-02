
#include "y/memory/digest.hpp"

namespace  Yttrium
{
    Digest:: ~Digest() noexcept
    {
        memset(workspace, 0, allocated);
    }
}
