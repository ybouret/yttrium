

#include "y/concurrent/pipeline/multiplex/mill.hpp"
#include "y/concurrent/pipeline/queue.hpp"


namespace Yttrium
{
    namespace Concurrent
    {
        MultiplexMill:: MultiplexMill(const Sequential_    &) :
        mill( Queue::Create() )
        {
        }

    }

}

