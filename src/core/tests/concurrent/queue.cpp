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
            //std::cerr << "[#Demo] @" << param << std::endl;
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

        inline void doMul(const Concurrent::ThreadContext &ctx, const int x)
        {
            {
                Y_LOCK(ctx.sync);
                (std::cerr << "[*Demo] @" << param << " in " << ctx.name << " => x" << x << " = " << param*x << std::endl).flush();
            }
            WallTime tmx;
            tmx.wait(0.1);
        }


        inline void getMul(const Concurrent::ThreadContext &ctx, int &res, const int x)
        {
            {
                Y_LOCK(ctx.sync);
                (std::cerr << "[*Demo] get @" << param << " in " << ctx.name << " => x" << x << " = " << (res = param*x) << std::endl).flush();
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
        (std::cerr << "Process(" << value << ")" << " in " << ctx.name << std::endl).flush();
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

    typedef Concurrent::Mission<NullType> Chore;

    Chore            task1(Functionoid,dem);
    Chore            task2(CxxMethodOf,dem, & Demo::unfold);
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
        const Chore            task(Functionoid,todo);
        tsk << task;
    }

    queue.push(tid,tsk);
    
    queue.flush();

    Y_THREAD_MSG("Ready to restart");

    

    {
        int value = 89;
        void (*proc)(const Concurrent::ThreadContext & , const int  ) = Process;
        typedef Concurrent::Mission<TL1(int)> IM;
        {
            const IM task(Functionoid,proc,value);
            alone.push(task);
            queue.push(task);
            queue.flush();
        }

        {
            const IM task(CxxMethodOf,dem, & Demo::doMul,value);
            alone.push(task);
            queue.push(task);
            queue.flush();
        }

        typedef Concurrent::Mission<TL2(int&,int)> GM;
        std::cerr << "#args=" << GM::ArgsType::Value << std::endl;
        {
            int res = 0 ;
            const GM task(CxxMethodOf,dem, & Demo::getMul,res,3);
            alone.push(task);
            std::cerr << "res=" << res << std::endl;
            queue.push(task);
            queue.flush();
            std::cerr << "res=" << res << std::endl;
        }


#if 0
        dem.param = 8;
        
        alone.call(dem);
        queue.call(dem);

        queue.flush();

        alone.invoke(dem, & Demo::unfold);
        queue.invoke(dem, & Demo::unfold);
        queue.flush();

        alone.call(proc,value);
        {
            Y_LOCK(queue.sync());
            (std::cerr << "invoke demo/value" << std::endl).flush();
        }
        alone.invoke(dem, & Demo::doMul, value);

        queue.flush();

        value   = 4;
        int res = 0;
        int *pRes = & res;
        alone.invoke(dem, & Demo::getMul, pRes, value);
#endif

    }


}
Y_UDONE()

