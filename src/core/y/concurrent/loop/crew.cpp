#include "y/concurrent/loop/crew.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        Crew:: ~Crew() noexcept
        {
        }


        Crew:: Crew(const Topology &topology) :
        Loop(),
        Threads(topology),
        item(agent-1)
        {
        }

        
        const char * const Crew::CallSign = "Concurrent::Crew";
        
        const char * Crew:: callSign() const noexcept { return CallSign; }

        size_t Crew:: size() const noexcept { return level; }

        Crew::ConstType & Crew:: operator[](const size_t indx) const noexcept
        {
            assert(indx>=1);
            assert(indx<=level);
            assert(indx==item[indx].indx);
            return item[indx];
        }

    }

}

