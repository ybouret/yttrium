

#include "y/concurrent/threads.hpp"

namespace Yttrium
{

    
    namespace Concurrent
    {
        Threads:: ~Threads() noexcept
        {
        }

        Threads:: Threads(const Topology &topo) :
        Agency(topo.size),
        access(),
        waitCV(),
        agents(0),
        agent( lead() )
        {

            for(size_t rank=0;rank<topo.size;++rank)
            {
            }


        }

        
    }

}
