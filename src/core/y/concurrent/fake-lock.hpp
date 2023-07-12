//! \file

#ifndef Yttrium_Concurrent_Fake_Lock_Included
#define Yttrium_Concurrent_Fake_Lock_Included 1


#include "y/lockable.hpp"


namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! FakeLock to only count calls
    //
    //
    //__________________________________________________________________________
    class FakeLock : public Lockable
    {
    public:
        explicit FakeLock(const char *id) noexcept; //!< setup
        virtual ~FakeLock() noexcept;               //!< cleanup

    private:
        Y_DISABLE_COPY_AND_ASSIGN(FakeLock);
        Y_LOCKABLE_API();
        void *impl; //!< only for padding
    };
}

#endif

