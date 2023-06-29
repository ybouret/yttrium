#include "y/type/utils.hpp"
#include <cstring>

namespace Yttrium
{
    size_t LengthOf(const char *s) noexcept
    {
        return s ? strlen(s) : 0;
    }

}
