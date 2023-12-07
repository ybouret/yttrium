#include "y/mkl/tao/seq/level1.hpp"
#include "y/mkl/tao/par/level1.hpp"

#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"
#include "y/sequence/vector.hpp"


#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    
}

Y_UTEST(tao)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;


    Concurrent::SharedLoop seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop parLoop = new Concurrent::Crew(topo);

    Tao::Carver seq(seqLoop);
    Tao::Carver par(parLoop);

    Vector<double> target(8,0);
    Vector<int>    source(10,0);
    for(size_t i=1;i<=source.size();++i) source[i] = int(i);

    std::cerr << "target=" << target << std::endl;
    std::cerr << "source=" << source << std::endl;
    Tao::Load(target,source);
    std::cerr << "target=" << target << std::endl;
    target.ld(0);
    std::cerr << "target=" << target << std::endl;
    Tao::Load(target,source,seq);
    std::cerr << "target=" << target << std::endl;
    target.ld(0);
    Tao::Load(target,source,par);
    std::cerr << "target=" << target << std::endl;





}
Y_UDONE()


