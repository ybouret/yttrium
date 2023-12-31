#include "y/concurrent/resource/in0d.hpp"
#include "y/concurrent/resource/in1d.hpp"
#include "y/concurrent/resource/in2d.hpp"
#include "y/concurrent/resources.hpp"

#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/queue.hpp"

#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/loop/crew.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;

namespace
{


    class Res0 : public Concurrent::Resource0D
    {
    public:
        explicit Res0(const Concurrent::ThreadContext &ctx) : Concurrent::Resource0D(ctx) {}
        virtual ~Res0() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Res0);
        virtual void activate() {}
        virtual void shutdown() noexcept {}
    };

    class Res1 : public Concurrent::Resource1D<int>
    {
    public:
        explicit Res1(const Concurrent::ThreadContext &ctx) : Concurrent::Resource1D<int>(ctx) {}
        virtual ~Res1() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Res1);
        virtual void activate() {}
        virtual void shutdown() noexcept {}
    };

    class Res2 : public Concurrent::Resource2D<int>
    {
    public:
        explicit Res2(const Concurrent::ThreadContext &ctx) : Concurrent::Resource2D<int>(ctx) {}
        virtual ~Res2() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Res2);
        virtual void activate() {}
        virtual void shutdown() noexcept {}
    };



}

Y_UTEST(concurrent_resource)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;

    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);
    Concurrent::SharedPipeline seqPipe = new Concurrent::Alone();
    Concurrent::SharedPipeline parPipe = new Concurrent::Queue(topo);

    std::cerr << std::endl;
    Concurrent::Resources<Res0> seq0(seqPipe);
    Concurrent::Resources<Res0> par0(parPipe);
    std::cerr << "seq0=" << seq0 << std::endl;
    std::cerr << "par0=" << par0 << std::endl;
    seq0.init();
    par0.init();
    std::cerr << "seq0=" << seq0 << std::endl;
    std::cerr << "par0=" << par0 << std::endl;


    std::cerr << std::endl;
    Concurrent::Resources<Res1> seq1(seqLoop);
    Concurrent::Resources<Res1> par1(parLoop);
    std::cerr << "seq1=" << seq1 << std::endl;
    std::cerr << "par1=" << par1 << std::endl;

    seq1.init(1,100,2);
    par1.init(1,100,2);
    std::cerr << "seq1=" << seq1 << std::endl;
    std::cerr << "par1=" << par1 << std::endl;




    std::cerr << std::endl;
    Concurrent::Resources<Res2> seq2(seqLoop);
    Concurrent::Resources<Res2> par2(parLoop);
    std::cerr << "seq2=" << seq2 << std::endl;
    std::cerr << "par2=" << par2 << std::endl;

    Res2::Vertex lower(1,1);
    Res2::Vertex upper(4,5);
    seq2.init(lower,upper);
    par2.init(lower,upper);
    std::cerr << "seq2=" << seq2 << std::endl;
    std::cerr << "par2=" << par2 << std::endl;


#if 0
    Res2::Vertex bad(0,0);
    seq2.init(lower,bad);
    par2.init(lower,bad);
    std::cerr << "seq2=" << seq2 << std::endl;
    std::cerr << "par2=" << par2 << std::endl;
#endif




}
Y_UDONE()

