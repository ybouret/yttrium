#include "y/concurrent/threads.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
       

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
        level(0),
        agent( lead() ),
        done_(0),
        waitCV(),
        doneCV()
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

                {
                    const Topology::NodeType *node = topology.head;
                    while(level<goal)
                    {
                        assert(0!=node);
                        Agent *a = new (&agent[level]) Agent(goal,level,access,*this);

                        access.lock();
                        if(done_>level)
                        {
                            // already done
                            access.unlock();
                        }
                        else
                        {
                            // wait for done (a.k.a thread is up)
                            doneCV.wait(access);
                            access.unlock();
                        }
                        // update
                        ++Coerce(level);

                        // assign
                        a->assign(**node);
                        node=node->next;
                    }
                }

                
                //--------------------------------------------------------------
                //
                // all set!
                //
                //--------------------------------------------------------------
                Y_THREAD_MSG( "[Threads] -------- ready and waiting #" << level );
            }
            catch(...)
            {
                quit();
                throw;
            }


        }


        void Threads:: quit() noexcept
        {
            Y_THREAD_MSG("[Threads] -------- quit #" << level);

            //------------------------------------------------------------------
            //
            // wake up all threads
            //
            //------------------------------------------------------------------
            waitCV.broadcast();

            //------------------------------------------------------------------
            //
            // join in destructor
            //
            //------------------------------------------------------------------
            while(level>0)
                Memory::OutOfReach::Naught( &agent[--Coerce(level)] );

            Y_THREAD_MSG("[Threads] -------- done...");
        }

        Threads:: ~Threads() noexcept
        {
            quit();
        }



        void Threads:: Launch(Threads &threads, Agent &agent) noexcept { threads.mainLoop(agent); }

        void Threads:: mainLoop(Agent &agent) noexcept
        {
            //------------------------------------------------------------------
            //
            // initializing thread
            //
            //------------------------------------------------------------------
            access.lock();
            ++done_;
            Y_THREAD_MSG("[Threads] startup " << agent.name);


            //------------------------------------------------------------------
            //
            // signaling Threads that this thread was successfully built!
            //
            //------------------------------------------------------------------
            if(done_>level) doneCV.signal();

            //------------------------------------------------------------------
            //
            // wait on a LOCKED mutex
            //
            //------------------------------------------------------------------
            waitCV.wait(access);

            //------------------------------------------------------------------
            //
            // wake on a LOCKED mutex
            //
            //------------------------------------------------------------------
            Y_THREAD_MSG("[Threads] woke up " << agent.name);


            access.unlock();
        }


    }

}
