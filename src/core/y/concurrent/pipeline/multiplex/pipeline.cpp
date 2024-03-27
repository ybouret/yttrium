#include "y/concurrent/pipeline/multiplex/pipeline.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        MultiplexPipeline:: ~MultiplexPipeline() noexcept { Coerce(Q) = 0; }

        MultiplexPipeline:: MultiplexPipeline(const SharedPipeline &sp) noexcept :
        Q( & Coerce(*sp) )
        {
        }

        Pipeline *        MultiplexPipeline:: operator->() noexcept       { return Q; }
        const  Pipeline * MultiplexPipeline:: operator->() const noexcept { return Q; }

    }

}

