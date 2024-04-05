
#include "y/concurrent/loop/simt/execute.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/utest/run.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

#include "y/concurrent/frame/1d.hpp"
#include "y/concurrent/frame/2d.hpp"
#include "y/system/wtime.hpp"
#include "y/random/bits.hpp"




using namespace Yttrium;


namespace
{

    class X1D : public Concurrent::Frame1D<size_t>
    {
    public:

        inline explicit X1D(const ThreadContext &ctx) noexcept : Frame1D<size_t>(ctx)
        {
        }

        inline virtual ~X1D() noexcept
        {
        }



    private:
        Y_DISABLE_ASSIGN(X1D);
    };

    inline void DoSomething0(X1D &range)
    {
        {
            Y_LOCK(range.sync);
            (std::cerr << "\tDoSomething0(" << X1D::LoopPtr(range.loop) << ")" << std::endl).flush();
        }
    }

}




Y_UTEST(concurrent_simtx)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");

    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);

    {

        Concurrent::SIMT<X1D> seq( seqLoop );
        Concurrent::SIMT<X1D> par( parLoop );

        std::cerr << "-- Testing" << std::endl;
        seq();
        par();
        std::cerr << std::endl;

        seq.assign(1,10,2);
        par.assign(1,10,2);
        std::cerr << "-- Testing 0-arg" << std::endl;
        seq(DoSomething0);
        par(DoSomething0);

    }

}
Y_UDONE()
