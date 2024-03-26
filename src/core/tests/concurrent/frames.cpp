#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/split/for-loop.hpp"
#include "y/concurrent/split/tiling.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

#include "y/concurrent/frame/punctual.hpp"
#include "y/concurrent/frame/1d.hpp"
#include "y/concurrent/frame/2d.hpp"


#include "y/concurrent/frames.hpp"

#include "y/utest/run.hpp"



using namespace Yttrium;

namespace
{
    class Demo0D : public Concurrent::PunctualFrame
    {
    public:
        explicit Demo0D(const ThreadContext &ctx) noexcept: Concurrent::PunctualFrame(ctx)
        {
        }

        virtual ~Demo0D() noexcept
        {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo0D);


    };

    class Demo1D : public Concurrent::Frame1D<size_t>
    {
    public:

        explicit Demo1D(const ThreadContext &ctx) noexcept: Concurrent::Frame1D<size_t>(ctx)
        {
        }

        virtual ~Demo1D() noexcept
        {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo1D);
        
    };


    class Demo2D : public Concurrent::Frame2D<unit_t>
    {
    public:

        explicit Demo2D(const ThreadContext &ctx) noexcept: Concurrent::Frame2D<unit_t>(ctx)
        {
        }

        virtual ~Demo2D() noexcept
        {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo2D);

    };




}


Y_UTEST(concurrent_frames)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;

    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);
    Concurrent::SharedPipeline seqPipe = new Concurrent::Alone();
    Concurrent::SharedPipeline parPipe = new Concurrent::Queue(topo);

    {

        Concurrent::Frames<Demo0D> f0seq(seqPipe);
        Concurrent::Frames<Demo0D> f0par(parPipe);

        std::cerr << "f0seq=" << f0seq << std::endl;
        std::cerr << "f0par=" << f0par << std::endl;


        f0seq.attach();
        f0par.attach();

        std::cerr << "f0seq=" << f0seq << std::endl;
        std::cerr << "f0par=" << f0par << std::endl;

    }

    {
        Concurrent::Frames<Demo1D> f1seq(seqLoop);
        Concurrent::Frames<Demo1D> f1par(parLoop);

        std::cerr << "f1seq=" << f1seq << std::endl;
        std::cerr << "f1par=" << f1par << std::endl;


        f1seq.attach(1,10,2);
        f1par.attach(1,10,2);

        std::cerr << "f1seq=" << f1seq << std::endl;
        std::cerr << "f1par=" << f1par << std::endl;

    }

    {
        Concurrent::Frames<Demo2D> f2seq(seqLoop);
        Concurrent::Frames<Demo2D> f2par(parLoop);
        std::cerr << "f2seq=" << f2seq << std::endl;
        std::cerr << "f2par=" << f2par << std::endl;

        const V2D<unit_t> lower(1,1);
        const V2D<unit_t> upper(10,10);

        f2seq.attach( lower, upper );
        f2par.attach( lower, upper );

        std::cerr << "f2seq=" << f2seq << std::endl;
        std::cerr << "f2par=" << f2par << std::endl;
    }

}
Y_UDONE()

