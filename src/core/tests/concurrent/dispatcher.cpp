
#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/queue.hpp"



//#include "y/concurrent/resources.hpp"
//#include "y/concurrent/pipeline/task.hpp"
#include "y/concurrent/pipeline/dispatcher/invoke.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"
#include "y/system/wtime.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/bits.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;


class MyEngine : public WallTime, public Random::Rand
{
public:

    explicit MyEngine() {}

    virtual ~MyEngine() noexcept
    {

    }

    void Proc0(const Concurrent::ThreadContext &ctx)
    {
        double s = 0;
        {
            Y_LOCK(ctx.sync);
            s = 0.1*to<double>();
            (std::cerr << "Proc0(" << ctx.name << ") : ... " << s << std::endl).flush();
        }
        wait(s);
    }

    void Proc1(const Concurrent::ThreadContext &ctx,
               const int x)
    {
        double s = 0;
        {
            Y_LOCK(ctx.sync);
            s = 0.1*to<double>();
            (std::cerr << "Proc1(" << ctx.name << ") : " << x << " ..." << s << std::endl).flush();
        }
        wait(s);
    }

    void Proc2(const Concurrent::ThreadContext &ctx,
               int       &res,
               const int  arg)
    {
        double s = 0;
        {
            Y_LOCK(ctx.sync);
            s = 0.1*to<double>();
            (std::cerr << "Proc2(" << ctx.name << ") : " << arg << " ..." << s << std::endl).flush();
        }
        wait(s);
        res = arg;
    }



private:
    Y_DISABLE_COPY_AND_ASSIGN(MyEngine);
};



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

    // 0 arg
    Concurrent::Invoke<MyEngine,NullType>::Call(ST, & MyEngine::Proc0);

    for(size_t i=1;i<=10;++i)
        Concurrent::Invoke<MyEngine,NullType>::Call(MT, & MyEngine::Proc0);

    MT.flush();

    // 1 arg
    Concurrent::Invoke<MyEngine,TL1(int)>::Call(ST, & MyEngine::Proc1,007);

    for(int i=1;i<=10;++i)
    {
        Concurrent::Invoke<MyEngine,TL1(int)>::Call(MT, & MyEngine::Proc1,i);
    }
    MT.flush();

    // 2 args
    {
        int res = 0;
        Concurrent::Invoke<MyEngine,TL2(int&,int)>::Call(ST, & MyEngine::Proc2,res,007);
        std::cerr << "res=" << res << std::endl;
    }

    {
        Vector<int> res(40,0);
        Vector<int> out(40,0);
        for(size_t i=res.size();i>0;--i)
        {
            Concurrent::Invoke<MyEngine,TL2(int&,int)>::Call(MT, & MyEngine::Proc2,res[i], out[i] = ran.in<int>(-10,10));
        }
        MT.flush();
        //std::cerr << res << "/" << out << std::endl;
        Y_CHECK(res==out);
    }






}
Y_UDONE()
