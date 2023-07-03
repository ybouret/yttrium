


#include "y/concurrent/mutex.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(concurrent_mutex)
{

    Lockable &giant = Concurrent::Mutex::Giant();
    std::cerr << giant.uuid << std::endl;

    {
        Y_LOCK(giant);
        std::cerr << "guarded..." << std::endl;
    }

}
Y_UDONE()

