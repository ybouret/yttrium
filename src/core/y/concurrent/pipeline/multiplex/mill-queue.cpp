#include "y/concurrent/pipeline/multiplex/mill.hpp"
#include "y/concurrent/pipeline/queue.hpp"


namespace Yttrium
{
    namespace Concurrent
    {
        MultiplexMill:: MultiplexMill(const InParallel_   &) :
        mill( Queue::Create() )
        {
        }

    }

}

