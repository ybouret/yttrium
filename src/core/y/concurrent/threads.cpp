

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
        doneCV(),
        size(0),
        crew( lead() ),
        nrun(0)
        {


            try 
            {


                //--------------------------------------------------------------
                //
                // setting goal
                //
                //--------------------------------------------------------------
                const size_t goal = topology.size;
                Y_THREAD_MSG( "[Threads] -------- initialize for Topology=" << topology << ", #" << goal);

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

                Y_THREAD_MSG( "[Threads] -------- ready and waiting #" << size );
            }
            catch(...)
            {
                quit();
                throw;
            }


        }


        void Threads:: quit() noexcept
        {
            Y_THREAD_MSG("[Threads] -------- quit #" << size);

            // wake up all threads
            waitCV.broadcast();

            // and join while destructing
            while(size>0)
                Memory::OutOfReach::Naught( &crew[--size] );

            Y_THREAD_MSG("[Threads] -------- done");
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
