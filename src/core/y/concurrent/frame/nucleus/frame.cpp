
#include "y/concurrent/frame/nucleus/frame.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        namespace Nucleus
        {
            Frame:: ~Frame() noexcept {}

            Frame:: Frame(const ThreadContext &ctx) noexcept : ThreadContext(ctx) {}
            
        }

    }
}

