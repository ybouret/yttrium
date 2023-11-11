


#include "y/concurrent/condition.hpp"
#include "y/concurrent/wire.hpp"
#include "y/concurrent/topology.hpp"
#include "y/system/hw.hpp"
#include "y/utest/run.hpp"
#include "y/data/list/cxx.hpp"
#include "y/random/bits.hpp"
#include "y/text/ascii/convert.hpp"

using namespace Yttrium;

namespace Yttrium
{
    namespace Concurrent
    {

        class Worker : public Object
        {
        public:

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Worker);
        };

    }
}


Y_UTEST(concurrent_queue)
{
    Concurrent::Thread::Verbose = true;
    const Concurrent::Topology topology;
    std::cerr << topology << std::endl;

    if(topology.size<=0) throw Exception("empty topology");


    std::cerr << "Master Thread   @" << Concurrent::Thread::CurrentHandle() << std::endl;


}
Y_UDONE()

