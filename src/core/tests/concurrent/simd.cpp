#include "y/concurrent/loop/simd.hpp"

#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/loop/crew.hpp"

#include "y/concurrent/engine/in1d.hpp"

#include "y/concurrent/topology.hpp"
#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;

namespace Yttrium
{
    class Tao1D : public Concurrent::Engine1D<size_t>
    {
    public:
        explicit Tao1D() noexcept {}
        virtual ~Tao1D() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Tao1D);
        virtual void activate(const Concurrent::ThreadContext &cntx)
        {
            std::cerr << "Activating Context " << cntx.name << std::endl;
        }
    };
}

Y_UTEST(concurrent_simd)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");

    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);

    Concurrent::SIMD<Tao1D>    seq( seqLoop );
    Concurrent::SIMD<Tao1D>    par( parLoop );

    std::cerr << std::endl;
    std::cerr << "seq=" << seq << std::endl;
    seq.dispatch(1,10,1);
    std::cerr << "seq=" << seq << std::endl;
    seq();

    std::cerr << std::endl;
    std::cerr << "par=" << par << std::endl;
    par.dispatch(1,10,1);
    std::cerr << "par=" << par << std::endl;
    par();



}
Y_UDONE()
