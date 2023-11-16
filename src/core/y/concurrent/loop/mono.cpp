
#include "y/concurrent/loop/mono.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        Mono:: ~Mono() noexcept
        {
        }

        Mono:: Mono(const char *id) noexcept :
        access(id),
        context(access)
        {
        }

    }

}

