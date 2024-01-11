#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/thread.hpp"
#include "y/utest/run.hpp"
#include "y/string/env.hpp"
#include "y/system/wtime.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;

namespace
{
    class Demo
    {
    public:
        inline  Demo(const int args) : param(args)
        {
            std::cerr << "[+Demo] @" << param << std::endl;
        }

        inline ~Demo() noexcept {}
        inline  Demo(const Demo &dem) noexcept :
        param(dem.param)
        {
            std::cerr << "[#Demo] @" << param << std::endl;
        }


        inline void unfold(const Concurrent::ThreadContext &ctx)
        {
            {
                Y_LOCK(ctx.sync);
                (std::cerr << "[*Demo] @" << param << " in " << ctx.name << std::endl).flush();
            }
            WallTime tmx;
            tmx.wait(0.1);
        }

        inline void operator()(const Concurrent::ThreadContext &ctx)
        {
            unfold(ctx);
        }


        int param;

    private:
        Y_DISABLE_ASSIGN(Demo);
    };

    static inline void Process(const Concurrent::ThreadContext &ctx, const int value)
    {
        Y_LOCK(ctx.sync);
        (std::cerr << "Process(" << value << ")" << std::endl).flush();
        WallTime tmx;
        tmx.wait(0.1);
    }

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

    {
        Concurrent::Task::ID tid = 0;
        tid = alone.push(task1); std::cerr << "tid = " << tid << std::endl;
        tid = alone.push(task2); std::cerr << "tid = " << tid << std::endl;
        tid = alone.push(task3); std::cerr << "tid = " << tid << std::endl;
        tid = alone.push(task4); std::cerr << "tid = " << tid << std::endl;
    }

    Concurrent::Queue  queue(topology);
    for(size_t i=0;i<18;++i)
    {
        (void) queue.push(task1);
    }

    queue.flush();

    queue.reset();
    Y_THREAD_MSG("Ready to restart");
    Vector<Concurrent::Task>     tsk;
    Vector<Concurrent::Task::ID> tid;
    for(int i=1;i<=40;++i)
    {
        const Demo             todo(i);
        const Concurrent::Task task(todo);
        tsk << task;
    }

    queue.push(tid,tsk);
    
    queue.flush();

    Y_THREAD_MSG("Ready to restart");

    {
        const int value = 89;
        void (*proc)(const Concurrent::ThreadContext & , const int  ) = Process;
        const Concurrent::Task task(Functionoid,proc,value);
        alone.push(task);
        queue.push(task);
    }




}
Y_UDONE()

