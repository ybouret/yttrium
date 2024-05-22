
#include "y/concurrent/pipeline/multiplex/mill.hpp"
#include "y/concurrent/pipeline/alone.hpp"


namespace Yttrium
{
    namespace Concurrent
    {
        MultiplexMill:: MultiplexMill(const Sequential_    &) :
        mill( new Alone() )
        {
        }
        
    }

}

