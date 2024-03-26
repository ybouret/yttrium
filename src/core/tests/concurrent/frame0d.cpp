
#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/frame/punctual.hpp"
#include "y/concurrent/frames.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;

namespace
{
    class Demo : public Concurrent::PunctualFrame
    {
    public:
        inline explicit Demo(const Concurrent::ThreadContext &ctx) noexcept :
        Concurrent::PunctualFrame(ctx)
        {}

        inline virtual ~Demo() noexcept {}
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo);
    };
}

Y_UTEST(concurrent_frame0d)
{

    const Concurrent::Topology topo;
    Concurrent::SharedPipeline seqEngine = new Concurrent::Alone();
    Concurrent::SharedPipeline parEngine = new Concurrent::Queue(topo);

    Concurrent::Frames<Demo> seq(seqEngine);
    Concurrent::Frames<Demo> par(parEngine);

    std::cerr << "Empty" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;

    seq.attach();
    par.attach();
    std::cerr << "Attached" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;

    seq.detach();
    par.detach();
    std::cerr << "Detached" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;


}
Y_UDONE()

