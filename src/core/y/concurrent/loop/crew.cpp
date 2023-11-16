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
        start(),
        ready(0),
        item(agent-1),
        code(this, & Crew::kMain)
        {
            std::cerr << "---- Crew Is Ready ----" << std::endl;
            once(code);
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

        void Crew:: kMain(const ThreadContext &ctx) noexcept
        {
            {
                Y_GIANT_LOCK();
                std::cerr << "Crew(" << ctx.name << ") @" << Thread:: CurrentHandle() <<  std::endl;
            }

            access.lock();
            ++ready;

            start.wait(access);


        }

    }

}

