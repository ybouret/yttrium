

#include "y/concurrent/condition.hpp"
#include "y/concurrent/wire.hpp"
#include "y/concurrent/topology.hpp"
#include "y/system/hw.hpp"
#include "y/utest/run.hpp"
#include "y/data/list/cxx.hpp"
#include "y/random/bits.hpp"
#include "y/text/ascii/convert.hpp"

using namespace Yttrium;

namespace
{
    class Barrier
    {
    public:
        explicit Barrier()  : mutex(), cond(), count(0), sum(0), meg(50)
        {
        }

        virtual ~Barrier() noexcept
        {
        }

        bool ready(const size_t nt) noexcept
        {
            Y_LOCK(mutex);
            return count >= nt;
        }

        Concurrent::Mutex     mutex;
        Concurrent::Condition cond;
        size_t                count;
        double                sum;
        size_t                meg;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Barrier);
    };

    static inline void MyProc(Barrier &barrier)
    {

        Random::Rand ran;

        {
            Y_LOCK(barrier.mutex);

            ++barrier.count;
            {
                Y_GIANT_LOCK();
                std::cerr << "Thread with barrier @" << barrier.count << std::endl;
            }
            barrier.cond.wait(barrier.mutex);// waiting on a locked mutex
            std::cerr << "Computing in thread @" << Concurrent::Thread::CurrentHandle() << std::endl;
        }

        // UNLOCKED computation
        double sum = 0;
        for(size_t i=barrier.meg * 1000000;i>0;--i)
        {
            sum += cos(100*ran.to<double>());
        }

        {
            Y_LOCK(barrier.mutex);
            std::cerr << "Done Computing..." << std::endl;
            barrier.sum += sum;
        }
    }

    class Worker : public Object, public Concurrent::Wire
    {
    public:
        explicit Worker(Barrier &barrier) :
        Object(),
        Concurrent::Wire(MyProc,barrier),
        uuid( handle() ),
        next(0), prev(0)
        {
            Y_GIANT_LOCK();
            std::cerr << "[Worker]        @" << handle() << std::endl;
        }

        virtual ~Worker() noexcept
        {
        }

        const Concurrent::ThreadHandle uuid;
        Worker  *next;
        Worker  *prev;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Worker);
    };


}


Y_UTEST(concurrent_topo)
{
    Concurrent::Thread::Verbose = true;
    const Concurrent::Topology topology;
    std::cerr << topology << std::endl;

    if(topology.size<=0) throw Exception("empty topology");



    const size_t      nt = topology.size;
    Barrier           barrier;
    CxxListOf<Worker> crew;
    for(size_t i=0;i<nt;++i)
    {
        crew.pushTail( new Worker(barrier) );
    }

    while( !barrier.ready(nt) )
        ;

    std::cerr << "-- ready!" << std::endl;

    topology.assign(crew);

    if(argc>1)
    {
        barrier.meg = ASCII::Convert::To<size_t>(argv[1],"mega cycles");
    }
    const Concurrent::ThreadHandle primary = Concurrent::Thread::CurrentHandle();
    {
        Y_GIANT_LOCK();
        std::cerr << "Master Thread   @" << primary << std::endl;
    }
    barrier.cond.broadcast();


}
Y_UDONE()

