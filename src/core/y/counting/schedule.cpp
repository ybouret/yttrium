
#include "y/counting/schedule.hpp"

namespace Yttrium
{
    Schedule:: ~Schedule() noexcept {}

    Schedule:: Schedule(const Cardinality n) noexcept :
    Counting(n),
    Readable<size_t>()
    {
    }
    
}


