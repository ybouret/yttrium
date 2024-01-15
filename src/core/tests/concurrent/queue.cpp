
#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/pipeline/job.hpp"

#include "y/concurrent/thread.hpp"
#include "y/utest/run.hpp"
#include "y/string/env.hpp"
#include "y/system/wtime.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/bits.hpp"

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

        inline void doWork(const Concurrent::ThreadContext &, double &res, const int x)
        {
            res = x * param;
        }

        inline void operator()(const Concurrent::ThreadContext &ctx)
        {
            unfold(ctx);
        }


        int param;

    private:
        Y_DISABLE_ASSIGN(Demo);
    };



    static inline void Proc0(const Concurrent::ThreadContext &ctx)
    {
        {
            Y_LOCK(ctx.sync);
            (std::cerr << "Proc0()" << " in " << ctx.name << std::endl).flush();
        }
        WallTime tmx;
        tmx.wait(0.1);
    }

    static inline void Proc1(const Concurrent::ThreadContext &ctx, const int value)
    {
        {
            Y_LOCK(ctx.sync);
            (std::cerr << "Proc1(" << value << ")" << " in " << ctx.name << std::endl).flush();
        }
        WallTime tmx;
        tmx.wait(0.1);
    }

    static inline void Proc2(const Concurrent::ThreadContext &ctx, int &res, const int value)
    {
        {
            Y_LOCK(ctx.sync);
            (std::cerr << "Proc2(" << value << ")" << " in " << ctx.name << std::endl).flush();
        }
        res = value * 2;
    }


}


 

Y_UTEST(concurrent_queue)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topology;
    std::cerr << topology << std::endl;
    
    Demo               dem(3);
    Random::Rand       ran;
    Concurrent::Alone  alone;
    Concurrent::Queue  queue(topology);


    typedef Concurrent::Job<NullType> Job0;
    void                    (*proc0)(const Concurrent::ThreadContext &) = Proc0;


    std::cerr << "-- Testing TaskUUID" << std::endl;
    {
        const Job0 j1(Functionoid,dem);
        const Job0 j2(CxxMethodOf,dem, & Demo::unfold);
        const Job0 j3 = j1;
        const Job0 j4 = j2;

        {
            Concurrent::TaskUUID tid = 0;
            tid = alone.load(j1); std::cerr << "tid = " << tid << std::endl;
            tid = alone.load(j2); std::cerr << "tid = " << tid << std::endl;
            tid = alone.load(j3); std::cerr << "tid = " << tid << std::endl;
            tid = alone.load(j4); std::cerr << "tid = " << tid << std::endl;
        }
    }

    Job0::Launch(alone,dem);
    Job0::Launch(alone,proc0);
    Job0::Invoke(alone,dem, & Demo::unfold );

    Job0::Launch(queue,dem);
    Job0::Launch(queue,proc0);
    Job0::Invoke(queue,dem, & Demo::unfold );
    queue.flush();


    {
        Vector<Concurrent::Task>       tsk;
        Vector<Concurrent::TaskUUID>   tid;
        for(int i=1;i<=40;++i)
        {
            const Demo todo(i);
            const Job0 task(Functionoid,todo);
            tsk << task;
        }

        queue.push(tid,tsk);
        queue.flush();
    }

    typedef Concurrent::Job<TL1(int)> Job1;
    void                    (*proc1)(const Concurrent::ThreadContext &, const int) = Proc1;

    const int value = 31;
    Job1::Launch(alone,proc1,value);
    Job1::Invoke(alone,dem, & Demo:: doMul, value);


    Job1::Launch(queue,proc1,value);
    Job1::Invoke(queue,dem, & Demo:: doMul, value);
    queue.flush();


    typedef Concurrent::Job<TL2(int &, const int)> Job2;
    void                    (*proc2)(const Concurrent::ThreadContext &,
                                     int &, const int) = Proc2;

    {
        int res = 0;
        Job2::Launch(alone,proc2,res,value);
        std::cerr << "res=" << res << std::endl;

        res = 0;
        Job2::Invoke(alone,dem, & Demo::getMul, res, value);
        std::cerr << "res=" << res << std::endl;
    }

    {
        Vector<int> res(8,0);
        for(int i=1;i<=8;++i)
        {
            Job2::Launch(alone,proc2,res[i],i);
        }
        std::cerr << "res=" << res << std::endl;

        res.ld(0);

        for(int i=1;i<=8;++i)
        {
            Job2::Launch(queue,proc2,res[i],i);
        }
        queue.flush();
        std::cerr << "res=" << res << std::endl;
    }


#if 0
    typedef Concurrent::Mission<NullType> Chore;

    Chore            task1(Functionoid,dem);
    Chore            task2(CxxMethodOf,dem, & Demo::unfold);
    Concurrent::Task task3 = task1;
    Concurrent::Task task4 = task2;

    dem.param = 7;

    Concurrent::Alone  alone;

    {
        Concurrent::TaskID tid = 0;
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
    Vector<Concurrent::TaskID>   tid;
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

        {
            Y_LOCK(queue.sync());
            (std::cerr << "Hi-Level"<< std::endl).flush();
        }

        Chore::Call(alone,dem);
        Chore::Call(queue,dem);
        queue.flush();

        Chore::Invoke(alone, dem, & Demo::unfold);
        Chore::Invoke(queue, dem, & Demo::unfold);
        queue.flush();

        value = 31;
        IM::Call(alone,proc,value);
        IM::Call(queue,proc,value);
        queue.flush();

        {
            int res = 0;
            GM::Invoke(alone, dem, & Demo::getMul, res, 6);
            std::cerr << "res=" << res << std::endl;
            res = 0;
            GM::Invoke(queue, dem, & Demo::getMul, res, 6);
            queue.flush();
            std::cerr << "res=" << res << std::endl;
        }

    }

    {
        typedef Concurrent::Mission<TL2(double&,int)> Work;
        const int      n = ran.in<int>(1,10);
        Vector<double> vec(n,0);
        for(int i=1;i<=n;++i)
        {
            Work::Invoke(queue, dem, & Demo::doWork, vec[i], i);
        }
        queue.flush();
        std::cerr << "vec=" << vec << std::endl;
    }
#endif


}
Y_UDONE()

