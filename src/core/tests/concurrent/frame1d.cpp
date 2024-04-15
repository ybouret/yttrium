#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/frame/1d.hpp"
#include "y/concurrent/frames.hpp"
#include "y/concurrent/thread.hpp"
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
    Concurrent::Thread::Verbose = true;
    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqEngine = new Concurrent::Mono();
    Concurrent::SharedLoop     parEngine = new Concurrent::Crew(topo);

    
    Concurrent::Frames<Demo> seq(seqEngine);return 0;
    Concurrent::Frames<Demo> par(parEngine);

    

    std::cerr << "Empty" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;

    seq.assign(1,10,2);
    par.assign(1,10,2);
    std::cerr << "Attached" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;

    for(size_t i=1;i<=par.size();++i)
    {
        const Concurrent::Trek<size_t> &sub = *par[i];
        std::cerr << "sub[" << std::setw(4) << i << "] = " << sub << std::endl;
    }

    seq.loosen();
    par.loosen();
    std::cerr << "Detached" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;

}
Y_UDONE()

