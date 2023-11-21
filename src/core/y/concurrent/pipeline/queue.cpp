


#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/condition.hpp"
#include "y/data/list/cxx.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"

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
                explicit Worker(const size_t sz,
                                const size_t rk,
                                Lockable    &mx) :
                Object(),
                ThreadContext(sz,rk,mx),
                next(0),
                prev(0)
                {
                }

                Worker *next;
                Worker *prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Worker);
            };
        }


        class Queue :: Code : public Object
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
            sync(),
            team(),
            meta(topology.size)
            {
                const size_t sz = topology.size;
                try
                {
                    for(const Topology::NodeType *node=topology.head;node;node=node->next)
                    {
                        const size_t rk = team.size;
                        Worker      *wk = team.pushTail( new Worker(sz,rk,sync) );
                        Coerce(meta[team.size]) = wk;
                    }
                }
                catch(...)
                {
                    throw;
                }

            }


            inline virtual ~Code() noexcept {}


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Mutex             sync; //!< shared mutex
            CxxListOf<Worker> team; //!< all workers
            const Meta        meta; //!< store addresses

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

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
            
        }


        const char * const Queue::CallSign = "Concurrent::Queue";

        const char * Queue:: callSign() const noexcept { return CallSign; }

        size_t Queue:: size() const noexcept
        {
            assert(0!=code);
            return code->team.size;
        }

        Queue::ConstType & Queue:: operator[](const size_t indx) const noexcept
        {
            assert(0!=code);
            return *(code->meta[indx]);
        }




    }

}

