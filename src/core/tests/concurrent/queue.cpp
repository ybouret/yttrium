#include "y/concurrent/topology.hpp"
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
    const Concurrent::Topology topology;
    std::cerr << topology << std::endl;
    
    if(topology.size<=0) throw Exception("empty topology");
    
    

}
Y_UDONE()

