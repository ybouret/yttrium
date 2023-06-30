

#include "y/concurrent/fake-lock.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(lockable)
{
    Y_SIZEOF(Lockable);
    Y_SIZEOF(FakeLock);

    FakeLock keep(NULL);
    FakeLock guard("guard");

    std::cerr << "id=" << keep.uuid << ", " << guard.uuid << std::endl;

}
Y_UDONE()

