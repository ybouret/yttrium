
#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/queue.hpp"

#include "y/concurrent/resources.hpp"
#include "y/concurrent/resource/in0d.hpp"
#include "y/concurrent/pipeline/task.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"
#include "y/system/wtime.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/bits.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;


class MyEngine
{
public:

    explicit MyEngine() {}

    virtual ~MyEngine() noexcept
    {

    }

    void Proc0(const Concurrent::ThreadContext &ctx)
    {
        {
            Y_LOCK(ctx.sync);
            (std::cerr << "Proc0(" << ctx.name << "):" << std::endl).flush();
        }
    }

private:
    Y_DISABLE_COPY_AND_ASSIGN(MyEngine);
};

namespace Yttrium
{
    namespace Concurrent
    {


        template <typename ENGINE>
        class Compute : public Resource0D, public ENGINE
        {
        public:

            inline explicit Compute(const ThreadContext &ctx) :
            Resource0D(ctx), ENGINE()
            {
            }

            inline virtual ~Compute() noexcept
            {

            }

            virtual void activate() noexcept {
                const Context &ctx = *this;
                Y_THREAD_MSG("activate " << ctx.name);
            }

            virtual void shutdown() noexcept  {
                const Context &ctx = *this;
                Y_THREAD_MSG("shutdown " << ctx.name);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compute);
        };

        template <typename ENGINE, typename METHOD, typename TLIST>
        class Mission : public Runnable, public Binder<TLIST>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            Y_BINDER_ECHO(TLIST); //!< aliases
            Y_BINDER_ARGS(TLIST); //!< aliases
            typedef Compute<ENGINE>           ComputeEngine;
            typedef Writable< ComputeEngine > ComputeEngines;

            inline virtual ~Mission() noexcept {}

            inline explicit Mission(ComputeEngines &kernel,
                                   METHOD          method) noexcept :
            Runnable(), 
            Binder<TLIST>(),
            core(kernel),
            meth(method)
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mission);
            ComputeEngines &core;
            METHOD          meth;

            inline virtual void run(const ThreadContext &ctx)
            {
                static const ArgsType args = {};
                ComputeEngine        &host = core[ctx.indx];
                call(host,args);
            }

            inline void call(ComputeEngine &host, const Int2Type<0> &) { (host.*meth)(host);                    }
#if 0
            inline void call(const ThreadContext &ctx, const Int2Type<1> &) { (host.*meth)(ctx,arg1);                }
            inline void call(const ThreadContext &ctx, const Int2Type<2> &) { (host.*meth)(ctx,arg1,arg2);           }
            inline void call(const ThreadContext &ctx, const Int2Type<3> &) { (host.*meth)(ctx,arg1,arg2,arg3);      }
            inline void call(const ThreadContext &ctx, const Int2Type<4> &) { (host.*meth)(ctx,arg1,arg2,arg3,arg4); }
#endif

        };

        template <typename ENGINE,typename TLIST>
        class Invoke : public Task
        {
        public:
            typedef Compute<ENGINE>              ComputeEngine;
            typedef Writable< ComputeEngine >    ComputeEngines;

            virtual ~Invoke() noexcept {}

            template <typename METHOD>
            inline explicit Invoke(ComputeEngines &kernel,
                                   METHOD          method) :
            Task( new Mission<ENGINE,METHOD,TLIST>(kernel,method) )
            {
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(Invoke);
        };



        template <typename ENGINE>
        class Dispatcher : 
        public Resources< Compute<ENGINE> >,
        public TaskManager
        {
        public:
            typedef Compute<ENGINE> ComputeEngine;

            explicit Dispatcher(const Concurrent::SharedPipeline &shared) :
            Resources<ComputeEngine>(shared),
            pipeline(shared)
            {
                this->init();
            }

            virtual ~Dispatcher() noexcept
            {
                this->quit();
            }


            virtual TaskUUID load(const Task &task)
            {
                return pipeline->load(task);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dispatcher);
            Concurrent::SharedPipeline pipeline;
        };

    }
}

Y_UTEST(concurrent_dispatcher)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topology;
    std::cerr << topology << std::endl;

    Random::Rand                ran;
    Concurrent::SharedPipeline  alone = new Concurrent::Alone();
    Concurrent::SharedPipeline  queue = new Concurrent::Queue(topology);

    Concurrent::Dispatcher<MyEngine> ST(alone);
    Concurrent::Dispatcher<MyEngine> MT(queue);

    Concurrent::Invoke<MyEngine, NullType> task0(ST, & MyEngine::Proc0 );
    ST.load(task0);

}
Y_UDONE()
