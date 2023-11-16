#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


Y_UTEST(concurrent_loop)
{
    Concurrent::Thread::Verbose = true;
    
    Concurrent::Mono           mono("mono");
    const Concurrent::Topology topo;
    Concurrent::Crew           crew(topo);


}
Y_UDONE()

