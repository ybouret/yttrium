#include "y/concurrent/engines.hpp"
#include "y/concurrent/engine/in1d.hpp"
#include "y/concurrent/engine/in2d.hpp"

#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/loop/crew.hpp"

#include "y/concurrent/topology.hpp"
#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;

namespace
{
    class E1 : public Concurrent::Engine1D<int>
    {
    public:
        inline explicit E1() noexcept {}
        inline virtual ~E1() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(E1);
        virtual void activate(const Concurrent::ThreadContext &cntx)
        {
            std::cerr << "activating E1 for " << cntx.name << std::endl;
        }
    };


    class E2 : public Concurrent::Engine2D<int>
    {
    public:
        inline explicit E2() noexcept {}
        inline virtual ~E2() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(E2);
        virtual void activate(const Concurrent::ThreadContext &cntx)
        {
            std::cerr << "activating E2 for " << cntx.name << std::endl;
        }
    };


}


Y_UTEST(concurrent_engines)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);
    Concurrent::Engines<E1>    seqE1(seqLoop);
    Concurrent::Engines<E1>    parE1(parLoop);

    Concurrent::Engines<E2>    seqE2(seqLoop);
    Concurrent::Engines<E2>    parE2(parLoop);

    std::cerr << std::endl;
    std::cerr << "seqE1=" << seqE1 << std::endl;
    seqE1(1,20,2);
    std::cerr << "seqE1=" << seqE1 << std::endl;

    std::cerr << std::endl;
    std::cerr << "parE1=" << parE1 << std::endl;
    parE1(1,20,2);
    std::cerr << "parE1=" << parE1 << std::endl;

    const V2D<int> lower(1,1), upper(10,20);

    std::cerr << std::endl;
    std::cerr << "seqE2=" << seqE2 << std::endl;
    seqE2(lower,upper);
    std::cerr << "seqE2=" << seqE2 << std::endl;


    std::cerr << std::endl;
    std::cerr << "parE2=" << parE2 << std::endl;
    parE2(lower,upper);
    std::cerr << "parE2=" << parE2 << std::endl;
}
Y_UDONE()

