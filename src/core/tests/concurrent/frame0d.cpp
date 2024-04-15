
#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/pipeline/multiplex/invoke.hpp"

#include "y/concurrent/frame/0d.hpp"
#include "y/random/bits.hpp"
#include "y/system/wtime.hpp"
#include "y/utest/run.hpp"
#include "y/type/binder.hpp"



using namespace Yttrium;

namespace
{


    class Demo0 : public Concurrent::Frame0D
    {
    public:
        inline explicit Demo0(const Concurrent::ThreadContext &ctx) noexcept :
        Concurrent::Frame0D(ctx), ran(), tmx()
        {}

        void call0()
        {
            {
                Y_LOCK(sync);
                std::cerr << "   (*) demo[" << *this << "]" << std::endl;
            }
            tmx.wait( ran.to<double>() );
        }

        void operator()(void)
        {
            call0();
        }

        void call1(double x)
        {
            {
                Y_LOCK(sync);
                std::cerr << "   (*) demo[" << *this << "].double=" << x << std::endl;
            }
            tmx.wait( ran.to<double>() );
        }

        void call2(double &x, const int a)
        {
            {
                Y_LOCK(sync);
                std::cerr << "   (*) demo[" << *this << "].double=" << x << " .int=" << a << std::endl;
            }
            x += a;
            tmx.wait( ran.to<double>() );
        }


        inline virtual ~Demo0() noexcept {}

        Random::Rand ran;
        WallTime     tmx;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo0);
    };


}

Y_UTEST(concurrent_frame0d)
{

    const Concurrent::Topology topo;
    Concurrent::SharedPipeline seqEngine = new Concurrent::Alone();
    Concurrent::SharedPipeline parEngine = new Concurrent::Queue(topo);

    Concurrent::Multiplex<Demo0> seq(seqEngine);
    Concurrent::Multiplex<Demo0> par(parEngine);

    std::cerr << "Empty" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;

    seq.assign();
    par.assign();
    std::cerr << "Attached" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;


    typedef Concurrent::Invoke<Demo0,NullType> Invoke0;

    std::cerr << "Sequential/0" << std::endl;
    for(int i=1;i<=2;++i)
    {
        Invoke0::On(seq, & Demo0::call0);
    }
    seq->flush();
    std::cerr << std::endl;

    std::cerr << "Parallel/0" << std::endl;
    for(int i=1;i<=4;++i)
    {
        Invoke0::On(par, & Demo0::call0);
    }
    par->flush();
    std::cerr << std::endl;

    typedef Concurrent::Invoke<Demo0,TL1(double)> Invoke1;

    std::cerr << "Sequential/1" << std::endl;
    for(int i=1;i<=2;++i)
    {
        Invoke1::On(seq, & Demo0::call1, i);
    }
    seq->flush();
    std::cerr << std::endl;

    std::cerr << "Parallel/1" << std::endl;
    for(int i=1;i<=4;++i)
    {
        Invoke1::On(par, & Demo0::call1, i);
    }
    par->flush();
    std::cerr << std::endl;


    typedef Concurrent::Invoke<Demo0,TL2(double &, const int)> Invoke2;

    {
        double       arr[] = { 3, 2, 1, 0 };
        const size_t num = sizeof(arr)/sizeof(arr[0]);

        Core::Display(std::cerr, arr, num) << std::endl;
        for(size_t i=0;i<sizeof(arr)/sizeof(arr[0]);++i)
        {
            Invoke2::On(seq, & Demo0::call2, arr[i], int(i) );
        }
        seq->flush();
        Core::Display(std::cerr, arr, num) << std::endl;

        for(size_t i=0;i<sizeof(arr)/sizeof(arr[0]);++i)
        {
            Invoke2::On(par, & Demo0::call2, arr[i], int(i) );
        }
        par->flush();
        Core::Display(std::cerr, arr, num) << std::endl;

    }

    seq.loosen();
    par.loosen();
    std::cerr << "Detached" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;


}
Y_UDONE()

