
#include "y/concurrent/loop/simt/execute.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/utest/run.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

#include "y/concurrent/frame/2d.hpp"
#include "y/system/wtime.hpp"
#include "y/random/bits.hpp"

using namespace Yttrium;

namespace
{

    class Demo : public Concurrent::Frame2D<size_t>
    {
    public:

        inline explicit Demo(const ThreadContext &ctx) noexcept :
        Concurrent::Frame2D<size_t>(ctx)
        {
            assert(0==tile);
        }

        inline virtual ~Demo() noexcept
        {
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo);
    };
}

Y_UTEST(concurrent_simt2d)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");

    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);


    //--------------------------------------------------------------------------
    //
    // Tests in 2D
    //
    //--------------------------------------------------------------------------
    Concurrent::SIMT<Demo> seq(seqLoop);
    Concurrent::SIMT<Demo> par(parLoop);

    const V2D<size_t> lower(1,1);
    const V2D<size_t> upper(4,5);

    seq.assign(lower,upper);
    par.assign(lower,upper);
    for(size_t i=1;i<=par.size();++i)
    {
        const Demo &dem = par[i];
        std::cerr << dem << " @" << Demo::TilePtr(dem.tile) << std::endl;
    }




}
Y_UDONE()

