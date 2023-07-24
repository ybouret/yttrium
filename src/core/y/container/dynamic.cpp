
#include "y/container/dynamic.hpp"

namespace Yttrium
{
    Dynamic:: ~Dynamic() noexcept
    {
    }

    Dynamic:: Dynamic() noexcept
    {
    }

    void Dynamic:: ensure(const size_t minCapacity)
    {
        if(capacity()<minCapacity)
        {
            reserve(minCapacity-capacity());
        }
    }


}

