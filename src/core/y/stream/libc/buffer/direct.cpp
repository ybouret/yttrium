

#include "y/stream/libc/buffer/direct.hpp"

#include <cstdio>

namespace Yttrium
{

    namespace Libc
    {

        DirectBuffer:: ~DirectBuffer() noexcept
        {
        }

        DirectBuffer:: DirectBuffer() :
        SystemBuffer(),
        curr(entry),
        last(entry+bytes)
        {

        }
    }

}


