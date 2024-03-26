#include "y/concurrent/pipeline/multiplex/q.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        MultiplexQueue:: ~MultiplexQueue() noexcept { Coerce(Q) = 0; }

        MultiplexQueue:: MultiplexQueue(const SharedPipeline &sp) noexcept :
        Q( & Coerce(*sp) )
        {
        }

        Pipeline *        MultiplexQueue:: operator->() noexcept       { return Q; }
        const  Pipeline * MultiplexQueue:: operator->() const noexcept { return Q; }

    }

}

