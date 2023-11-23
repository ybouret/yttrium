


#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/condition.hpp"
#include "y/concurrent/wire.hpp"

#include "y/data/list/cxx.hpp"
#include "y/data/pool/cxx.hpp"

#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/sort/merge.hpp"
#include "y/ptr/auto.hpp"


namespace Yttrium
{
    namespace Concurrent
    {
        namespace
        {
            //__________________________________________________________________
            //
            //
            //
            //! Worker handled by Queue
            //
            //
            //__________________________________________________________________
            class Worker : public Object, public ThreadContext
            {
            public:

                //! setup context and launch code->run(*this) in thread
                explicit Worker(Queue::Code &code,
                                const size_t sz,
                                const size_t rk,
                                Lockable    &mx) :
                Object(),
                ThreadContext(sz,rk,mx),
                next(0),
                prev(0),
                cond(),
                wire(Launch,code,*this)
                {
                }

                //! wait for thread to return
                inline virtual ~Worker() noexcept {}


                Worker    *next; //!< for list
                Worker    *prev; //!< for list
                Condition  cond; //!< self waiting condition
                Wire       wire; //!< thread

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Worker);
                static void Launch(Queue::Code &, Worker &) noexcept;
            };
        }


        namespace
        {
            class Task : public Object
            {
            public:
                typedef CxxListOf<Task> List;
                typedef CxxPoolOf<Task> Pool;

                inline explicit Task() noexcept  :
                next(0), prev(0), job(0), jid(0)
                {
                }


                inline virtual ~Task() noexcept
                {
                    erase();
                }

                inline void erase() noexcept
                {
                    if(0!=job) delete job;
                    job = 0;
                    jid = 0;
                }


                Task  *next;
                Task  *prev;

                Job   *job;
                JobID  jid;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Task);
            };



        }

        class Queue :: Code : public Object, public ListOf<Worker>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxArray<Worker *,Memory::Dyadic> Meta; //!< array of worker indices

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit Code(const Topology &topology) :
            Object(),
            ListOf<Worker>(),
            sync(),
            meta(topology.size),
            tasks(),
            tpool(),
            count(0),
            fence()
            {
                Y_THREAD_MSG("[Queue] creating " << topology);
                const size_t sz = topology.size;
                try
                {
                    for(const Topology::NodeType *node=topology.head;node;node=node->next)
                    {
                        const size_t rk = size;
                        Worker      *wk = pushTail( new Worker(*this,sz,rk,sync) );
                        Coerce(meta[size]) = wk;
                        {
                            Y_LOCK(sync);
                            if(count<size) fence.wait(sync);
                        }
                        wk->wire.assign(**node);
                    }
                }
                catch(...)
                {
                    quit();
                    throw;
                }

                Y_THREAD_MSG("[Queue] synchronized #" << size);

            }


            inline virtual ~Code() noexcept {
                quit();
            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void run(Worker &) noexcept;

            inline void load(const Job &J)
            {

                Task *task = tpool.size>0 ? tpool.query() : new Task();
                assert(0!=task);
                assert(0==task->job);
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Mutex          sync;  //!< shared mutex
            const Meta     meta;  //!< store addresses
            Task::List     tasks; //!< loaded tasks
            Task::Pool     tpool; //!< empty  tasks
            size_t         count; //!< counter
            Condition      fence; //!< condition to use count

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
            inline void quit() noexcept
            {
                Y_THREAD_MSG("[Queue] quit...");



                //--------------------------------------------------------------
                // Then release all waiting
                //--------------------------------------------------------------
                MergeSort::ByIncreasingAddress(*this);
                while(size>0)
                {
                    Worker *w = popTail();
                    w->cond.broadcast();
                    delete w;
                }
            }



        };

        namespace
        {
            inline void Worker:: Launch(Queue::Code &code, Worker &worker) noexcept
            {
                code.run(worker);
            }
        }

        void Queue:: Code:: run(Worker &worker) noexcept
        {
            //------------------------------------------------------------------
            // First Synchronization
            //------------------------------------------------------------------
            sync.lock();
            if(++count>=size) fence.signal();

            //------------------------------------------------------------------
            // wait on a LOCKED mutex
            //------------------------------------------------------------------
            worker.cond.wait(sync);
            Y_THREAD_MSG("[Queue] waking up " << worker.name);
            sync.unlock();
        }

    }

}

#include "y/type/nullify.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        Queue:: ~Queue() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        Queue:: Queue(const Topology &topology) :
        Pipeline(),
        code( new Code(topology) )
        {
            std::cerr << " *** sizeof(Code)   = " << sizeof(Code) << std::endl;
            std::cerr << " *** sizeof(Worker) = " << sizeof(Worker) << std::endl;
        }


        const char * const Queue::CallSign = "Concurrent::Queue";

        const char * Queue:: callSign() const noexcept { return CallSign; }

        size_t Queue:: size() const noexcept
        {
            assert(0!=code);
            return code->size;
        }

        Queue::ConstType & Queue:: operator[](const size_t indx) const noexcept
        {
            assert(0!=code);
            return *(code->meta[indx]);
        }




    }

}

