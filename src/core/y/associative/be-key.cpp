
#include "y/associative/be-key.hpp"
#include <cstring>

namespace Yttrium
{
    BE_Key:: ~BE_Key() noexcept
    {
        memset(code,0,sizeof(code));
    }

    BE_Key:: BE_Key(const BE_Key &other) noexcept
    {
        memcpy(code,other.code,sizeof(code));
    }

}

