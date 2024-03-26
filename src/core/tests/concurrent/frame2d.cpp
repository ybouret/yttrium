
#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/frame/2d.hpp"
#include "y/concurrent/frames.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;

namespace
{
    class Demo : public Concurrent::Frame2D<unit_t>
    {
    public:
        inline explicit Demo(const Concurrent::ThreadContext &ctx) noexcept :
        Concurrent::Frame2D<unit_t>(ctx)
        {}

        inline virtual ~Demo() noexcept {}
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo);
    };
}


Y_UTEST(concurrent_frame2d)
{
    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqEngine = new Concurrent::Mono();
    Concurrent::SharedLoop     parEngine = new Concurrent::Crew(topo);

    Concurrent::Frames<Demo> seq(seqEngine);
    Concurrent::Frames<Demo> par(parEngine);

    std::cerr << "Empty" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;

    const V2D<unit_t> lower(1,1);
    const V2D<unit_t> upper(10,8);

    seq.attach(lower,upper);
    par.attach(lower,upper);
    std::cerr << "Attached" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;
 
    for(size_t i=1;i<=par.size();++i)
    {
        std::cerr << "sub[" << std::setw(4) << i << "] = " << *par[i] << std::endl;
    }


    seq.detach();
    par.detach();
    std::cerr << "Detached" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;
}
Y_UDONE()

