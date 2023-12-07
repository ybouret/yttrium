

#include "y/concurrent/loop/simt2d.hpp"
#include "y/concurrent/thread.hpp"

#include "y/utest/run.hpp"
#include "y/container/matrix.hpp"


#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/loop/crew.hpp"



#include "y/concurrent/thread.hpp"
#include "y/concurrent/topology.hpp"
#include "y/string/env.hpp"

using namespace Yttrium;

namespace
{
    class Tao2D : public Concurrent::Engine2D<size_t>
    {
    public:
        explicit Tao2D() noexcept {}
        virtual ~Tao2D() noexcept {}

    private:
        virtual void activate(const Concurrent::ThreadContext &cntx)
        {
            std::cerr << "in " << cntx.name << " : " << *this << std::endl;
        }
    };

    inline void DoSomething(Tao2D &range)
    {
        Y_LOCK( range.sync() );
        std::cerr << "In " << Concurrent::Thread::CurrentHandle() << std::endl;
        const Tao2D::Tile &tile = *range;
        for(size_t i=0;i<tile.size;++i)
        {
            const Tao2D::Segment &s = tile[i];
            std::cerr << "\t" << s << std::endl;
        }
    }
}

Y_UTEST(concurrent_simt2d)
{
    Concurrent::Thread::Verbose     = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);

    Concurrent::SIMT2D<Tao2D> seq(seqLoop);
    Concurrent::SIMT2D<Tao2D> par(parLoop);

    const size_t nrow = 5; // y
    const size_t ncol = 8; // x

    V2D<size_t> lower(1,1);
    V2D<size_t> upper(ncol,nrow);

    seq.dispatch(lower,upper);
    par.dispatch(lower,upper);

    std::cerr << "seq: " << std::endl;
    seq( DoSomething );

    std::cerr << std::endl;
    std::cerr << "par: " << std::endl;
    par( DoSomething );


}
Y_UDONE()

