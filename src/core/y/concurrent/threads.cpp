

#include "y/concurrent/threads.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        Agent:: ~Agent() noexcept
        {

        }

        Agent:: Agent(const size_t sz, const size_t rk, Lockable &mx, Threads &threads) :
        ThreadContext(sz,rk,mx),
        Wire(Threads::Launch,threads,*this)
        {

        }

    }

    namespace Concurrent
    {
        Threads:: ~Threads() noexcept
        {
        }

        Threads:: Threads(const Topology &topo) :
        Agency(topo.size),
        access(),
        waitCV(),
        size(0),
        crew( lead() ),
        nrun(0)
        {

            while(size<topo.size)
            {
                Agent *a = new (&crew[size]) Agent(topo.size,size,access,*this);
                ++size;
            }



        }

        void Threads:: Launch(Threads &threads, Agent &agent) noexcept { threads.loop(agent); }

        void Threads:: loop(Agent &agent) noexcept
        {

            // initializing thread
            access.lock();
            std::cerr << "locked by " << agent.name << std::endl;

            // first wait
            waitCV.wait(access);


        }


    }

}
