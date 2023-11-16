
#include "y/concurrent/thread/context.hpp"

namespace Yttrium
{

    namespace Concurrent
    {
        ThreadContext:: ~ThreadContext() noexcept
        {
        }

        ThreadContext:: ThreadContext(const ThreadContext &ctx) noexcept :
        Context(ctx),
        sync(ctx.sync)
        {
        }

        ThreadContext:: ThreadContext(const size_t sz, const size_t rk, Lockable &mx) noexcept :
        Context(sz,rk), sync(mx)
        {
        }

        
    }

}
