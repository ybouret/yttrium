
#include "y/text/plural.hpp"

namespace Yttrium
{

    const char * Plural::S(const size_t n) noexcept
    {
        return n > 1 ? "S" : "";
    }

    const char * Plural::s(const size_t n) noexcept
    {
        return n > 1 ? "s" : "";
    }

}

