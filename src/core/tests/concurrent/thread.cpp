#include "y/concurrent/mutex.hpp"
#include "y/concurrent/thread.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;


namespace
{

    struct Param
    {
        Lockable &access;
        size_t    value;
    };

    static inline void theProc(void *args)
    {
        assert(0!=args);
        Param &param = *static_cast<Param *>(args);

        Y_LOCK(param.access);
        std::cerr << "in thread #" << param.value << std::endl;
    }

}

Y_UTEST(concurrent_thread)
{

    Lockable          &access = Lockable::Giant();
    Param              p1 = { access, 1 };
    Param              p2 = { access, 2 };
    Concurrent::Thread t1(theProc,&p1);
    Concurrent::Thread t2(theProc,&p2);

}
Y_UDONE()

