#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/thread.hpp"
#include "y/utest/run.hpp"
#include "y/string/env.hpp"

using namespace Yttrium;

namespace
{
    class Demo
    {
    public:
        inline  Demo(const int args) : param(args) {}
        inline ~Demo() noexcept {}
        inline  Demo(const Demo &dem) noexcept : param(dem.param) {}


        inline void unfold(const Concurrent::ThreadContext &ctx)
        {
            {
                Y_LOCK(ctx.sync);
                (std::cerr << "demo #" << param << " in " << ctx.name << std::endl).flush();
            }
        }

        inline void operator()(const Concurrent::ThreadContext &ctx)
        {
            unfold(ctx);
        }


        int param;

    private:
        Y_DISABLE_ASSIGN(Demo);
    };
}

Y_UTEST(concurrent_queue)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topology;
    std::cerr << topology << std::endl;
    
    Demo  dem(3);

    Concurrent::Task task1(dem);
    Concurrent::Task task2(dem, & Demo::unfold);
    Concurrent::Task task3 = task1;
    Concurrent::Task task4 = task2;

    dem.param = 7;

    Concurrent::Alone  alone;

    Concurrent::Task::ID tid = 0;
    tid = alone.push(task1); std::cerr << "tid = " << tid << std::endl;
    tid = alone.push(task2); std::cerr << "tid = " << tid << std::endl;
    tid = alone.push(task3); std::cerr << "tid = " << tid << std::endl;
    tid = alone.push(task4); std::cerr << "tid = " << tid << std::endl;
    
    Concurrent::Queue  queue(topology);
    for(size_t i=0;i<18;++i)
    {
        tid = queue.push(task1);
    }

    queue.flush();


}
Y_UDONE()

