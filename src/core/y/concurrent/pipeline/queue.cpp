


#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/condition.hpp"
#include "y/concurrent/wire.hpp"

#include "y/data/list.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/sort/merge.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        namespace
        {
            //! Worker handled by Queue
            class Worker : public Object, public ThreadContext
            {
            public:
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


                Worker    *next;
                Worker    *prev;
                Condition  cond;
                Wire       wire;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Worker);
                static void Launch(Queue::Code &, Worker &) noexcept;
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

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Mutex          sync;  //!< shared mutex
            const Meta     meta;  //!< store addresses
            size_t         count; //!< counter
            Condition      fence;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
            inline void quit() noexcept
            {
                Y_THREAD_MSG("[Queue] quit...");

                //--------------------------------------------------------------
                // then release all waiting
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
            std::cerr << "sizeof(Code)=" << sizeof(Code) << std::endl;
            std::cerr << "sizeof(Worker)=" << sizeof(Worker) << std::endl;
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

