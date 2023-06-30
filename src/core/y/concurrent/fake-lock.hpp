//! \file

#ifndef Yttrium_Concurrent_Fake_Lock_Included
#define Yttrium_Concurrent_Fake_Lock_Included 1


#include "y/lockable.hpp"


namespace Yttrium
{
    class FakeLock : public Lockable
    {
    public:
        explicit FakeLock(const char *id) noexcept;
        virtual ~FakeLock() noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(FakeLock);
        Y_LOCKABLE_API();
        void *impl;
    };
}

#endif

