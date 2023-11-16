

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

        Threads:: Threads(const Topology &topology) :
        Agency(topology.size),
        access(),
        waitCV(),
        size(0),
        crew( lead() ),
        nrun(0)
        {
            if(Thread::Verbose)
            {
                Y_GIANT_LOCK();
                std::cerr << "[Threads] initialize for Topology=" << topology << ", #" << topology.size << std::endl;
            }

            try {
                const size_t goal = topology.size;
                while(size<goal)
                {
                    Agent *a = new (&crew[size]) Agent(goal,size,access,*this);

                    access.lock();
                    if(nrun>size)
                    {
                        access.unlock();
                    }
                    else
                    {
                        // wait one done
                        doneCV.wait(access);
                        access.unlock();
                    }
                    ++size;
                }
            }
            catch(...)
            {

                throw;
            }
            if(Thread::Verbose)
            {
                Y_GIANT_LOCK();
                std::cerr << "[Threads] ready and waiting" << std::endl;
            }
        }


        void Threads:: quit() noexcept
        {
            if(Thread::Verbose)
            {
                Y_GIANT_LOCK();
                std::cerr << "[Threads] quit #" << size << std::endl;
            }

            // wake up all threads
            waitCV.broadcast();

            // and join
            while(size>0)
            {
                Memory::OutOfReach::Naught( &crew[--size] );
            }
        }

        Threads:: ~Threads() noexcept
        {
            quit();
        }



        void Threads:: Launch(Threads &threads, Agent &agent) noexcept { threads.loop(agent); }

        void Threads:: loop(Agent &agent) noexcept
        {

            // initializing thread
            access.lock();
            ++nrun;
            {
                Y_GIANT_LOCK();
                std::cerr << "[Threads] locked by " << agent.name << std::endl;
            }

            if(nrun>size) doneCV.signal();

            // wait on a LOCKED mutex
            waitCV.wait(access);

            // wake on a LOCKED mutex
            {
                Y_GIANT_LOCK();
                std::cerr << "[Threads] woke up " << agent.name << std::endl;
            }

            access.unlock();

        }


    }

}
