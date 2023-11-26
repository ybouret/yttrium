#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/condition.hpp"
#include "y/concurrent/wire.hpp"

#include "y/data/list/cxx.hpp"
#include "y/data/pool/cxx.hpp"

#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/sort/merge.hpp"

#include "y/memory/out-of-reach.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        namespace
        {

            //__________________________________________________________________
            //
            //
            //! a Job stores a Task and its ID
            //
            //__________________________________________________________________
            class Job
            {
            public:
                //______________________________________________________________
                //
                // Definitions
                //______________________________________________________________
                typedef ListOf<Job> List; //!< alias
                typedef PoolOf<Job> Pool; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline  Job(const Task &t, const Task::ID u) noexcept : next(0), prev(0), task(t), uuid(u) {} //!< setup
                inline ~Job() noexcept {} //!< cleanup

                //______________________________________________________________
                //
                // Methods
                //______________________________________________________________

                //! helper to cleanup completed job
                static inline Job *Zombified(Job *active) noexcept
                {
                    assert(0!=active); assert(0==active->next); assert(0==active->prev);
                    return static_cast<Job *>( Memory::OutOfReach::Naught(active) );
                }

                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                Job           *next; //!< for list
                Job           *prev; //!< for list
                const Task     task; //!< shared task
                const Task::ID uuid; //!< task UUID

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Job);
            };

            //__________________________________________________________________
            //
            //
            //! self-container list of jobs
            //
            //__________________________________________________________________
            class JList : public Job::List
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline explicit JList() noexcept : Job::List(), zpool() {} //!< setup empty
                inline virtual ~JList() noexcept { crush(); prune(); }     //!< cleanup

                //______________________________________________________________
                //
                // <ethods
                //______________________________________________________________
                inline void clear() noexcept { while(size>0)         dismiss( popTail() );                        } //!< dismiss list, keep memory in pool
                inline void prune() noexcept { while( size>0 )       Object::zrelease( Destructed( popTail() ) ); } //!< delete list
                inline void crush() noexcept { while( zpool.size>0 ) Object::zrelease( zpool.query() );           } //!< delete pool


                //! enqueue a new task
                inline Task::ID enqueue(const Task    &task,
                                        const Task::ID uuid)
                {
                    return pushTail( new (zpool.size > 0 ? zpool.query() : Object::zacquire<Job>()) Job(task,uuid) )->uuid;
                }

                //! cleanup and store job in zpool
                inline void dismiss(Job *active) noexcept { zpool.store( Job::Zombified(active) ); }

                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                Job::Pool zpool; //!< pool of zombi jobs

            private:
                Y_DISABLE_COPY_AND_ASSIGN(JList);
            };

        }

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
                typedef ListOf<Worker> List;


                //! setup context and launch code->run(*this) in thread
                explicit Worker(Queue::Code &code,
                                const size_t sz,
                                const size_t rk,
                                Lockable    &mx) :
                Object(),
                ThreadContext(sz,rk,mx),
                next(0),
                prev(0),
                duty(0),
                cond(),
                wire(Launch,code,*this)
                {
                }


                inline void resume() noexcept
                {
                    cond.broadcast();
                }

                //! wait for thread to return
                inline virtual ~Worker() noexcept {}


                Worker    *next; //!< for list
                Worker    *prev; //!< for list
                Job       *duty; //!< job to do
                Condition  cond; //!< self waiting condition
                Wire       wire; //!< thread

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Worker);
                static void Launch(Queue::Code &, Worker &) noexcept;
            };
        }



        //______________________________________________________________________
        //
        //
        //
        //! Queue is a Waiting list of Workers
        //
        //
        //______________________________________________________________________
        class Queue :: Code : public Object, public Worker::List
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
            Worker::List(),
            sync(),
            meta(topology.size),
            busy(),
            jobs(),
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
            void run(Worker &) noexcept; //!< entry point
            void restart()     noexcept; //!< restart after enqueueing some tasks


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Mutex          sync;  //!< shared mutex
            const Meta     meta;  //!< store addresses
            Worker::List   busy;  //!< working
            JList          jobs;  //!< list of jobs to do
            size_t         count; //!< counter
            Condition      fence; //!< condition to use count

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
            inline void quit() noexcept
            {
                Y_THREAD_MSG("[Queue] quit...");

                //--------------------------------------------------------------
                // Deleting waiting jobs
                //--------------------------------------------------------------
                {
                    Y_LOCK(sync);
                    jobs.crush();

                }

                //--------------------------------------------------------------
                // waiting for busy workers to complete...
                //--------------------------------------------------------------


                //--------------------------------------------------------------
                // Then release all waiting
                //--------------------------------------------------------------
                MergeSort::ByIncreasingAddress(*this);
                while(size>0)
                {
                    Worker *w = popTail(); // remove from queue
                    w->resume();           // resume worker
                    delete w;              // join and delete
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


        //----------------------------------------------------------------------
        //
        //
        // threaded code
        //
        //
        //----------------------------------------------------------------------
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

            //------------------------------------------------------------------
            // waking on a LOCKED mutex
            //------------------------------------------------------------------
            Y_THREAD_MSG("[Queue] waking up " << worker.name);
            if(0!=worker.duty)
            {
            }
            else
            {
                Y_THREAD_MSG("[Queue] returning from " << worker.name);
                sync.unlock();
            }
        }

        // called from primary
        void Queue:: Code:: restart() noexcept
        {
            // called on a LOCKED mutex

            // dispatching enqueue jobs

            while(jobs.size>0 && size>0)
            {
                assert(0==tail->duty);
                Worker *w = busy.pushHead(popTail());
                w->duty   = jobs.popHead();
                w->resume();
            }

            // and UNLOCK
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
            std::cerr << " *** sizeof(Code)   = " << sizeof(Code)   << std::endl;
            std::cerr << " *** sizeof(Worker) = " << sizeof(Worker) << std::endl;
            std::cerr << " *** sizeof(Job)    = " << sizeof(Job)    << std::endl;

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

        void Queue:: suspend() noexcept
        {
            assert(0!=code);
            code->sync.lock();
        }

        void Queue:: restart() noexcept
        {
            assert(0!=code);
            code->restart();
        }

        Task::ID Queue:: enqueue(const Task &task, const Task::ID uuid)
        {
            // assuming LOCKED mutex
            assert(0!=code);
            return code->jobs.enqueue(task,uuid);
        }



    }

}

