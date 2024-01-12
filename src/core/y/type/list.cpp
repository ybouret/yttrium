#include "y/type/list.hpp"
#include <iostream>

namespace Yttrium
{
    const char * const NullType:: CallSign = "NullType";
    std::ostream & operator<<(std::ostream &os, const NullType &)
    {
        return os << NullType::CallSign;
    }
}

namespace Yttrium
{
    const char * const EmptyType:: CallSign = "EmptyType";
    std::ostream & operator<<(std::ostream &os, const EmptyType &)
    {
        return os << EmptyType::CallSign;
    }
}
