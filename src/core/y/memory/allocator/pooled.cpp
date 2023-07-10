
#include "y/memory/allocator/pooled.hpp"

namespace Yttrium
{

    namespace Memory
    {
        const AtExit::Longevity Pooled:: LifeTime = AtExit::MaximumLongevity - 1;
        const char * const      Pooled:: CallSign = "Memory::Pooled";



    }

}

