

#include "y/concurrent/condition.hpp"
#include "y/concurrent/wire.hpp"
#include "y/system/hw.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

namespace
{
    class Barrier
    {
    public:
        explicit Barrier(const size_t ini)  : mutex(), cond(), count(ini)
        {
        }

        virtual ~Barrier() noexcept
        {
        }


        Concurrent::Mutex     mutex;
        Concurrent::Condition cond;
        size_t                count;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Barrier);
    };

    static inline void MyProc(Barrier &barrier)
    {
        {
            Y_LOCK(barrier.mutex);
            std::cerr << "Thread with barrier @" << barrier.count << std::endl;
            if(--barrier.count>0)
            {
                barrier.cond.wait(barrier.mutex);
                // wake up on a locked mutex
                std::cerr << "Waking up!" << std::endl;
            }
            else
            {
                // mutex is locked
                std::cerr << "Will launch computation!" << std::endl;
                barrier.cond.broadcast();
            }
        }


    }


}


Y_UTEST(concurrent_topo)
{

    std::cerr << "#procs=" << Hardware::NumProcs() << std::endl;



}
Y_UDONE()

