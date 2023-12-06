
#include "y/concurrent/loop/simt1d.hpp"

#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/loop/crew.hpp"

#include "y/concurrent/thread.hpp"
#include "y/concurrent/topology.hpp"
#include "y/string/env.hpp"

#include "y/sequence/vector.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;

namespace
{
    class Tao1D : public Concurrent::Engine1D<size_t>
    {
    public:
        explicit Tao1D() noexcept {}
        virtual ~Tao1D() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Tao1D);
        Vector<double> data;

        virtual void activate(const Concurrent::ThreadContext &cntx)
        {
            const size_t n = length;
            data.adjust(n,0);
            std::cerr << "in " << cntx.name << " : " << *this << std::endl;
        }
    };

    

}


Y_UTEST(concurrent_simt1d)
{
    Concurrent::Thread::Verbose     = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);

    Concurrent::SIMT1D<Tao1D> seq( seqLoop );
    Concurrent::SIMT1D<Tao1D> par( parLoop );

    seq.dispatch(1,100,2);
    par.dispatch(1,100,2);


}
Y_UDONE()

