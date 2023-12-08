
#include "y/concurrent/resource/interface.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {
            Resource:: ~Resource() noexcept {}


            Resource:: Resource(const ThreadContext &ctx) noexcept :
            ThreadContext(ctx)
            {

            }

            
        }

    }

}
