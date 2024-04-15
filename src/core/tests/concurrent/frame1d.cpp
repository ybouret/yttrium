#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/frame/1d.hpp"
#include "y/concurrent/frames.hpp"
#include "y/concurrent/thread.hpp"
#include "y/utest/run.hpp"
#include "y/string/env.hpp"

using namespace Yttrium;

namespace
{
    class Demo1 : public Concurrent::Frame1D<size_t>
    {
    public:
        inline explicit Demo1(const Concurrent::ThreadContext &ctx) noexcept :
        Concurrent::Frame1D<size_t>(ctx)
        {
            std::cerr << "New Demo " << name << " @" << (void *)this << std::endl;
        }

        inline virtual ~Demo1() noexcept {}

        int data[2];
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo1);
    };
}

Y_UTEST(concurrent_frame1d)
{

    Concurrent::SharedLoop     seqEngine = new Concurrent::Mono();

    std::cerr << "sizeof(Demo1)=" << sizeof(Demo1) << std::endl;
    std::cerr << "*seqEngine @" << & *seqEngine << std::endl;
    std::cerr << "seqEngine=" << seqEngine << std::endl;
    Concurrent::Frames<Demo1> seq(seqEngine);
    return 0;
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;
    Concurrent::SharedLoop     parEngine = new Concurrent::Crew(topo);
    Concurrent::Frames<Demo1> par(parEngine);

    

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

