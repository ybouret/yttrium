


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
        typedef uint32_t JobID;


        class Worker : public Object
        {
        public:

            Worker *next;
            Worker *prev;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Worker);
        };

        class Queue
        {
        public:
            explicit Queue(const Topology &topo);
            virtual ~Queue() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Queue);
            Mutex     access;
            Condition waitCV;
        };

        Queue:: Queue(const Topology &) :
        access(),
        waitCV()
        {

        }

        Queue:: ~Queue() noexcept
        {
            
        }

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

