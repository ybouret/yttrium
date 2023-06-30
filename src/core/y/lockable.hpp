
//! \file

#ifndef Yttrium_Lockable_Included
#define Yttrium_Lockable_Included 1


#include "y/config/starting.hpp"


namespace Yttrium
{
    class Lockable
    {
    public:
        
        virtual ~Lockable() noexcept;
        void     lock()    noexcept;
        void     unlock()  noexcept;
        bool     tryLock() noexcept;

        const  int  depth;
        const  char uuid[64-2*sizeof(void*)-sizeof(int)];


    protected:
        explicit Lockable(const char *id) noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Lockable);
        virtual void doLock()    noexcept = 0;
        virtual void doUnlock()  noexcept = 0;
        virtual bool doTryLock() noexcept = 0;
    };

#define Y_LOCKABLE_API()           \
virtual void doLock()    noexcept; \
virtual void doUnlock()  noexcept; \
virtual bool doTryLock() noexcept

    class ScopedLock
    {
    public:
        ScopedLock(Lockable &) noexcept;
        ~ScopedLock() noexcept;
    private:
        Y_DISABLE_COPY_AND_ASSIGN(ScopedLock);
        Lockable &host;
    };

}

#endif

