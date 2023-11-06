



#include "y/concurrent/condition.hpp"
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

    

}


Y_UTEST(concurrent_condition)
{

    Barrier barrier(3);


}
Y_UDONE()

