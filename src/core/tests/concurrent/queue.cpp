


#include "y/concurrent/condition.hpp"
#include "y/concurrent/wire.hpp"
#include "y/concurrent/topology.hpp"
#include "y/system/hw.hpp"
#include "y/utest/run.hpp"
#include "y/data/list/cxx.hpp"
#include "y/random/bits.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/functor.hpp"

using namespace Yttrium;

namespace Yttrium
{
    namespace Concurrent
    {


        class Context
        {
        public:
            explicit Context(Lockable &mx, const size_t sz, const size_t rk) noexcept;
            explicit Context(Lockable &mx) noexcept; //!< sz=1, rk=0
            virtual ~Context() noexcept;
            Context(const Context &) noexcept;

            Lockable    &sync; //!< shared persistent  mutex
            const size_t size; //!< size>0
            const size_t rank; //!< 0..size-1
            const size_t indx; //!< 1..size

        private:
            Y_DISABLE_ASSIGN(Context);
        };

        Context:: Context(Lockable &mx, const size_t sz, const size_t rk) noexcept :
        sync(mx),
        size(sz),
        rank(rk),
        indx(rank+1)
        {
            assert(size>0); assert(rank<size);
        }

        Context:: Context(Lockable &mx) noexcept :
        sync(mx),
        size(1),
        rank(0),
        indx(1)
        {

        }

        Context:: ~Context() noexcept {}

        Context:: Context(const Context &ctx) noexcept :
        sync(ctx.sync),
        size(ctx.size),
        rank(ctx.rank),
        indx(ctx.indx)
        {
        }

        typedef int64_t                            JobID;
        typedef Functor<void,TL1(const Context &)> Job;

        class Demo
        {
        public:
            inline  Demo(int i) noexcept : code(i) {}
            inline ~Demo() noexcept {}
            Demo(const Demo &demo) noexcept : code(demo.code) {}

            const int code;

            inline void operator()(const Context &ctx)
            {
                {
                    Y_LOCK(ctx.sync);
                    std::cerr << "called by " << ctx.size << "." << ctx.rank << std::endl;
                    std::cerr << "code=" << code << std::endl;
                }
            }

        private:
            Y_DISABLE_ASSIGN(Demo);
        };


        class Queue;

        class Worker : public Object, public Context
        {
        public:
            typedef CxxListOf<Worker> List;
            explicit Worker(const Context &ctx) :
            Object(),
            Context(ctx),
            next(0),
            prev(0)
            {
            }

            virtual ~Worker() noexcept
            {
            }

            Worker *next;
            Worker *prev;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Worker);
        };

        class Queue
        {
        public:
            explicit Queue(const Topology &topo);
            virtual ~Queue() noexcept;

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Queue);
            Mutex        access;
            Condition    waitCV;
            Worker::List ready;

            void run() noexcept;
            static void  Launch( Queue &Q )
            {
                Q.run();
            }


        };

        Queue:: Queue(const Topology &topo) :
        access(),
        waitCV(),
        ready()
        {

            try {
                // creating workers
                const size_t sz = topo.size;
                for(size_t rk=0;rk<topo.size;++rk)
                {
                    const Context ctx(access,sz,rk);

                }
            }
            catch(...)
            {

                throw;
            }

        }

        Queue:: ~Queue() noexcept
        {
            
        }

        void Queue:: run() noexcept
        {

        }

    }
}


Y_UTEST(concurrent_queue)
{
    Concurrent::Thread::Verbose = true;
    const Concurrent::Topology topology;
    std::cerr << topology << std::endl;

    if(topology.size<=0) throw Exception("empty topology");


    const Concurrent::ThreadHandle th = Concurrent::Thread::CurrentHandle();
    std::cerr << "Master Thread   @" << th << std::endl;
    Y_SIZEOF(Concurrent::ThreadHandle);

    Concurrent::Demo demo = 7;
    Concurrent::Job j = demo;
    const Concurrent::Context   solo( Lockable::Giant() );
    j(solo);
}
Y_UDONE()

