


#include "y/concurrent/condition.hpp"
#include "y/concurrent/wire.hpp"
#include "y/concurrent/topology.hpp"
#include "y/system/hw.hpp"
#include "y/utest/run.hpp"
#include "y/data/list/cxx.hpp"
#include "y/random/bits.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/functor.hpp"

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
    
}
Y_UDONE()

