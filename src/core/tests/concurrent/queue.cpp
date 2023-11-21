#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/thread.hpp"
#include "y/utest/run.hpp"
#include "y/string/env.hpp"

using namespace Yttrium;

namespace Yttrium
{
    namespace Concurrent
    {

#if 0
        class Queue;

        class Worker : public Object
        {
        public:
            explicit Worker(Queue &Q) :
            Object(),
            myCV(),
            wire(Launch,Q,*this),
            next(0),
            prev(0)
            {
            }

            Condition myCV;
            Wire      wire;
            Worker   *next;
            Worker   *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Worker);
            void enroll(Queue &Q) noexcept;

            static
            void Launch(Queue &Q, Worker &w) noexcept
            {
                w.enroll(Q);
            }
        };

        class Queue
        {
        public:
            explicit Queue(const Topology &topo) :
            sync(),
            idle(),
            done(0),
            doneCV()
            {
                Y_THREAD_MSG("+Queue");
                try
                {
                    for(const Topology::NodeType *node=topo.head;node;node=node->next)
                    {
                        Worker *w = idle.pushTail( new Worker(*this) );

                        {
                            Y_LOCK(sync);
                            if(done<idle.size) doneCV.wait(sync);
                        }
                        Y_THREAD_MSG("ready ");

                    }
                }
                catch(...)
                {
                    throw;
                }

            }

            virtual ~Queue() noexcept
            {
                Y_THREAD_MSG("~Queue");
            }

            Mutex             sync;
            CxxListOf<Worker> idle;
            size_t            done;
            Condition         doneCV;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Queue);
        };

        void Worker:: enroll(Queue &Q) noexcept
        {
            Mutex &sync = Q.sync;

            sync.lock();
            if(++Q.done>=Q.idle.size) Q.doneCV.signal();

            myCV.wait(sync);
        }
#endif
    }
}


Y_UTEST(concurrent_queue)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topology;
    std::cerr << topology << std::endl;
    
    Concurrent::Alone  alone;
    Concurrent::Queue  queue(topology);


}
Y_UDONE()

