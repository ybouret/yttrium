#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/task.hpp"
#include "y/concurrent/thread.hpp"
#include "y/system/wtime.hpp"
#include "y/string/env.hpp"
#include "y/random/bits.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

namespace Yttrium
{

    namespace Concurrent
    {


        class MyJob : public Runnable, public WallTime
        {
        public:

            explicit MyJob() : Runnable(),  WallTime(), ran()
            {
                std::cerr << "+Job" << std::endl;
            }

            virtual ~MyJob() noexcept
            {
                std::cerr << "-Job" << std::endl;
            }

            virtual void run(const ThreadContext &ctx)
            {
                {
                    Y_LOCK(ctx.sync);
                    std::cerr << "run@" << ctx.name << std::endl;
                }
                wait( ran.to<double>() );
            }

            Random::Rand ran;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MyJob);
        };

        class Todo : public Task
        {
        public:
            explicit Todo() : Task( new MyJob() )
            {
            }

            virtual ~Todo() noexcept
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Todo);
        };


    }


}

Y_UTEST(concurrent_pipeline)
{
    std::cerr << "Preparing Job..." << std::endl;
    Concurrent::Todo todo;

    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topology;
    std::cerr << "topology=" << topology << std::endl;


    Random::Rand                ran;
    Concurrent::SharedPipeline  alone = new Concurrent::Alone();
    Concurrent::SharedPipeline  queue = new Concurrent::Queue(topology);


    (std::cerr << "Loading Sequential" << std::endl).flush();
    for(size_t i=2+ran.leq(2);i>0;--i)
    {
        alone->load(todo);
    }
    alone->flush();


    for(size_t i=5+ran.leq(5);i>0;--i)
    {
        queue->load(todo);
    }
    queue->flush();
    



}
Y_UDONE()
