
#include "y/concurrent/pipeline/multiplex/mill.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        MultiplexMill:: ~MultiplexMill() noexcept {}

        MultiplexMill:: MultiplexMill(const SharedPipeline &sp) noexcept :
        mill(sp)
        {
        }
        

        Pipeline *        MultiplexMill:: operator->()       noexcept { return & *mill; }
        const Pipeline  * MultiplexMill:: operator->() const noexcept { return & *mill; }
        
    }

}

