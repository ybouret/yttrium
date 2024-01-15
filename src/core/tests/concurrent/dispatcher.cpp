
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


class Computing
{
public:

    explicit Computing() {}

    virtual ~Computing() noexcept
    {

    }


private:
    Y_DISABLE_COPY_AND_ASSIGN(Computing);
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

        


        template <typename ENGINE>
        class Dispatcher : public Resources< Compute<ENGINE> >
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

            void load(const Task &task)
            {
                pipeline->load(task);
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

    Concurrent::Dispatcher<Computing> ST(alone);
    Concurrent::Dispatcher<Computing> MT(queue);

}
Y_UDONE()
