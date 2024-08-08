
#include "y/text/boolean.hpp"


namespace Yttrium
{
    const char * BooleanTo:: Text(const bool flag) noexcept
    {
        return flag ? "True" : "False";
    }

    const char * BooleanTo:: text(const bool flag) noexcept
    {
        return flag ? "true" : "false";
    }


}


