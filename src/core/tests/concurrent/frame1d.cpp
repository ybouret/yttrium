#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/frame/1d.hpp"
#include "y/concurrent/frames.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;

namespace
{
    class Demo : public Concurrent::Frame1D<size_t>
    {
    public:
        inline explicit Demo(const Concurrent::ThreadContext &ctx) noexcept :
        Concurrent::Frame1D<size_t>(ctx)
        {}

        inline virtual ~Demo() noexcept {}
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo);
    };
}

Y_UTEST(concurrent_frame1d)
{
    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqEngine = new Concurrent::Mono();
    Concurrent::SharedLoop     parEngine = new Concurrent::Crew(topo);

    Concurrent::Frames<Demo> seq(seqEngine);
    Concurrent::Frames<Demo> par(parEngine);

    std::cerr << "Empty" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;

    seq.attach(1,10,2);
    par.attach(1,10,2);
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

