

#include "y/concurrent/condition.hpp"
#include "y/concurrent/wire.hpp"
#include "y/concurrent/thread/handle-zip.hpp"

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
        explicit Barrier()  : mutex(), cond(), count(0), sum(0), meg(1)
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
                (std::cerr << "Thread with barrier @" << barrier.count << std::endl).flush();
            }
            barrier.cond.wait(barrier.mutex);// waiting on a locked mutex
            {
                Y_GIANT_LOCK();
                (std::cerr << "Computing in thread @" << Concurrent::Thread::CurrentHandle() << std::endl).flush();
            }
        }

        // UNLOCKED computation
        double sum = 0;
        for(size_t i=barrier.meg * 1000000;i>0;--i)
        {
            sum += cos(100*ran.to<double>());
        }

        {
            Y_GIANT_LOCK();
            (std::cerr << "Done Computing..." << std::endl).flush();
            barrier.sum += sum;
        }
    }

    class Worker : public Object, public Concurrent::Wire
    {
    public:
        explicit Worker(Barrier &barrier) :
        Object(),
        Concurrent::Wire(MyProc,barrier),
        next(0), prev(0)
        {
            Y_GIANT_LOCK();
            (std::cerr << "[Worker] " << handle << std::endl).flush();
        }

        virtual ~Worker() noexcept
        {
        }

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


    (void)topology.assign(crew);


    if(argc>1)
    {
        barrier.meg = ASCII::Convert::To<size_t>(argv[1],"mega cycles");
    }
    Concurrent::ThreadHandle primary = Concurrent::Thread::CurrentHandle();
    {
        Y_GIANT_LOCK();
        std::cerr << "Master Thread   @" << primary << std::endl;
    }


    if(true)
    {
        Concurrent::ThreadHandleZip thz;
        for(const Worker *w=crew.head;w;w=w->next)
        {
            thz << w->handle;
        }

        std::cerr << "#Handles = " << thz.size << std::endl;
        const size_t homology1 = thz.homology();
        std::cerr << "Homology1 = " << homology1 << std::endl;

        thz.loadPrimary();
        const size_t homology2 = thz.homology();
        std::cerr << "Homology2 = " << homology2 << std::endl;

        primary.skip(homology2);
        std::cerr << "New Primary: " << primary << std::endl;

        thz.unloadPrimary();
        assert(thz.homology() == homology1);
        Y_CHECK(thz.compress(homology1));
    }




    barrier.cond.broadcast();

    {
        Y_GIANT_LOCK();
        Y_SIZEOF(Concurrent::Thread);
        Y_SIZEOF(Concurrent::Wire);
    }

}
Y_UDONE()

