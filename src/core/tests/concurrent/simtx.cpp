
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

namespace Yttrium
{
    namespace Concurrent
    {

        template <typename T>
        class X1D : public Frame1D<T>
        {
        public:

            inline explicit X1D(const ThreadContext &ctx) noexcept : Frame1D<T>(ctx)
            {
            }

            inline virtual ~X1D() noexcept
            {
            }



        private:
            Y_DISABLE_ASSIGN(X1D);
        };

    }
}

using namespace Yttrium;


namespace
{

}




Y_UTEST(concurrent_simtx)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");

    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);

    typedef Concurrent::X1D<size_t> Engine1D;

    Concurrent::SIMT<Engine1D> seq( seqLoop );
    Concurrent::SIMT<Engine1D> par( parLoop );

    seq();
    par();
    

}
Y_UDONE()
