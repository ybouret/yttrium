#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/condition.hpp"
#include "y/concurrent/wire.hpp"
#include "y/concurrent/thread/handle-zip.hpp"

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
            //! a QJob stores a Task and its ID
            //
            //__________________________________________________________________
            class QJob
            {
            public:
                //______________________________________________________________
                //
                // Definitions
                //______________________________________________________________
                typedef ListOf<QJob> List; //!< alias
                typedef PoolOf<QJob> Pool; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline  QJob(const Task &t, const TaskUUID u) noexcept : next(0), prev(0), task(t), uuid(u) {} //!< setup
                inline ~QJob() noexcept {} //!< cleanup

                //______________________________________________________________
                //
                // Methods
                //______________________________________________________________

                //! helper to cleanup completed job
                static inline QJob *Zombified(QJob *active) noexcept
                {
                    assert(0!=active); assert(0==active->next); assert(0==active->prev);
                    return static_cast<QJob *>( Memory::OutOfReach::Naught(active) );
                }

                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                QJob           *next; //!< for list
                QJob           *prev; //!< for list
                Task            task; //!< shared task
                const TaskUUID  uuid; //!< task UUID

            private:
                Y_DISABLE_COPY_AND_ASSIGN(QJob);
            };

            //__________________________________________________________________
            //
            //
            //! self-container list of jobs
            //
            //__________________________________________________________________
            class JList : public QJob::List
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline explicit JList() noexcept : QJob::List(), zpool() {} //!< setup empty
                inline virtual ~JList() noexcept { crush(); prune(); }      //!< cleanup

                //______________________________________________________________
                //
                // <ethods
                //______________________________________________________________
                inline void clear() noexcept { while(size>0)         dismiss( popTail() );                        } //!< dismiss list, keep memory in pool
                inline void prune() noexcept { while( size>0 )       Object::zrelease( Destructed( popTail() ) ); } //!< delete list
                inline void crush() noexcept { while( zpool.size>0 ) Object::zrelease( zpool.query() );           } //!< delete pool


                //! enqueue a new task
                inline TaskUUID enqueue(const Task    &task,
                                        const TaskUUID uuid)
                {
                    return pushTail( new (zpool.size > 0 ? zpool.query() : Object::zacquire<QJob>()) QJob(task,uuid) )->uuid;
                }

                //! cleanup and store job in zpool
                inline void dismiss(QJob *active) noexcept { zpool.store( QJob::Zombified(active) ); }

                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                QJob::Pool zpool; //!< pool of zombi jobs

            private:
                Y_DISABLE_COPY_AND_ASSIGN(JList);
            };

            typedef JList::NodeType JNode;

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

                //! resume from waiting state
                inline void resume() noexcept { cond.broadcast(); }

                //! wait for thread to return
                inline virtual ~Worker() noexcept {}


                Worker    *next; //!< for list
                Worker    *prev; //!< for list
                QJob      *duty; //!< job to do
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
                tryZip();
            }

            //! cleanup
            inline virtual ~Code() noexcept { quit(); }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void run(Worker &) noexcept; //!< entry point
            void restart()     noexcept; //!< restart after enqueueing some tasks
            inline void flush() noexcept
            {
                Y_LOCK(sync);
                if(busy.size>0) fence.wait(sync);
            }

            Task::Status query(const TaskUUID uuid) const noexcept;

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
            void quit() noexcept;

            inline void tryZip() noexcept
            {
                try
                {
                    ThreadHandleZip thz( size );
                    for(const Worker *w=head;w;w=w->next)
                        thz << w->wire.handle;
                    thz.compress();
                }
                catch(...)
                {
                    Y_THREAD_MSG("[Queue] Failure to ThreadHandleZip");
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


        inline void Queue::Code:: quit() noexcept
        {
            Y_THREAD_MSG("[Queue] quit...");


            {
                //--------------------------------------------------------------
                // Deleting waiting jobs
                //--------------------------------------------------------------
                Y_LOCK(sync);
                Y_THREAD_MSG("[Queue] ---- clear #jobs=" << jobs.size);
                jobs.clear();
                assert(0==jobs.size);

            }

            flush();
            assert(0==busy.size);




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
            //
            // First Synchronization
            //
            //------------------------------------------------------------------
            const char * const  id = worker.name;
            const ThreadHandle &h  = worker.wire.handle;
            sync.lock();
            if(++count>=size) fence.signal(); // signal main thread we are done

            //------------------------------------------------------------------
            //
            // wait on a LOCKED mutex
            //
            //------------------------------------------------------------------
        CYCLE:
            worker.cond.wait(sync);

            //------------------------------------------------------------------
            //
            // waking on a LOCKED mutex
            //
            //------------------------------------------------------------------
            if(0!=worker.duty)
            {
            WORK:
                assert(0!=worker.duty);
                Y_THREAD_MSG("[Q:" << h << "@" << id << "] start job#" << worker.duty->uuid);
                assert(busy.owns(&worker));

                //--------------------------------------------------------------
                // perform UNLOCKED task
                //--------------------------------------------------------------
                sync.unlock();
                try
                {
                    Task &task = worker.duty->task;
                    task.process(worker);
                }
                catch(...)
                {

                }

                sync.lock();
                //--------------------------------------------------------------
                // LOCKED : dismiss job
                //--------------------------------------------------------------
                Y_THREAD_MSG("[Q:" << h << "@" << id << "] done  job#" << worker.duty->uuid);
                jobs.dismiss(worker.duty);

                //--------------------------------------------------------------
                // check what's next
                //--------------------------------------------------------------
                if(jobs.size>0)
                {
                    worker.duty = jobs.popHead();
                    goto WORK;
                }
                else
                {
                    worker.duty = 0;
                    pushTail(busy.pop(&worker));
                    if(busy.size<=0)
                        fence.signal(); // signal main thread no more busy worker
                    goto CYCLE;
                }

            }
            else
            {
                Y_THREAD_MSG("[Q:" << h << "@" << id << "] returning");
                sync.unlock();
            }
        }

        // called from primary
        void Queue:: Code:: restart() noexcept
        {
            // called on a LOCKED mutex

            // dispatching enqueued jobs
            while(jobs.size>0 && size>0)
            {
                assert(0==tail->duty);
                Worker *w = busy.pushHead(popTail());
                w->duty   = jobs.popHead();
                Y_THREAD_MSG("[Q:" << w->wire.handle << "@" << w->name << "] assigned job#" << w->duty->uuid);
                 w->resume();
            }

            // and UNLOCK
            sync.unlock();
        }

        Task::Status Queue:: Code:: query(const TaskUUID uuid) const noexcept
        {
            Y_LOCK(Coerce(sync));

            for(const JNode *node=jobs.head;node;node=node->next)
            {
                if(uuid==node->uuid) return Task::Pending;
            }

            for(const Worker *node=busy.head;node;node=node->next)
            {
                assert(0!=node->duty);
                if(uuid==node->duty->uuid) return Task::Running;
            }


            return Task::Success;
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
            std::cerr << " *** sizeof(QJob)   = " << sizeof(QJob)    << std::endl;

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

        TaskUUID Queue:: enqueue(const Task &task, const TaskUUID uuid)
        {
            // assuming LOCKED mutex
            assert(0!=code);
            return code->jobs.enqueue(task,uuid);
        }


        void Queue:: flush() noexcept
        {
            assert(0!=code);
            code->flush();
        }

        Task::Status Queue:: query(const TaskUUID uuid)  const noexcept
        {
            assert(0!=code);
            return code->query(uuid);
        }


        Lockable    & Queue:: sync() noexcept
        {
            assert(0!=code);
            return code->sync;
        }
    }

}

