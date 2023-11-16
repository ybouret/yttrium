


#include "y/concurrent/threads.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

namespace Yttrium
{
    namespace Concurrent
    {


    }
}


Y_UTEST(concurrent_queue)
{
    Concurrent::Thread::Verbose = true;
    const Concurrent::Topology topology;
    std::cerr << topology << std::endl;
    
    if(topology.size<=0) throw Exception("empty topology");
    
    
    const Concurrent::ThreadHandle th = Concurrent::Thread::CurrentHandle();
    std::cerr << "Master Thread   @" << th << std::endl;
    Y_SIZEOF(Concurrent::ThreadHandle);
    
    {
        Concurrent::Threads threads(topology);

    }


    Y_SIZEOF(Concurrent::Agent);
    
}
Y_UDONE()

