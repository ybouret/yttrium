
#include "y/concurrent/fake-lock.hpp"


namespace Yttrium
{
    FakeLock:: ~FakeLock() noexcept {}

    FakeLock:: FakeLock(const char *id) noexcept :
    Lockable(id),
    impl(0)
    {

    }

    void FakeLock:: doLock() noexcept
    {
    }

    void FakeLock:: doUnlock() noexcept
    {
    }

    bool FakeLock:: doTryLock() noexcept
    {
        return true;
    }
}
