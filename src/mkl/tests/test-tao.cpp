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
    //Concurrent::SharedLoop parLoop = new Concurrent::Crew(topo);

    Tao::Carver seq(seqLoop);




}
Y_UDONE()


