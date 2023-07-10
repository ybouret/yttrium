
//! \file

#ifndef Yttrium_Lockable_Included
#define Yttrium_Lockable_Included 1


#include "y/config/starting.hpp"


namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Lockable object interface
    //
    //
    //__________________________________________________________________________
    class Lockable
    {
    public:
        //______________________________________________________________________
        //
        //
        // definition
        //
        //______________________________________________________________________
        typedef int         DepthType;                                                //!< alias
        static const size_t DepthSize  = sizeof(DepthType);                           //!< alias
        static const size_t ClassSize  = 64;                                          //!< sizeof(DerivedClasses)
        static const size_t CXX_Bytes  = sizeof(void*);                               //!< virtual table
        static const size_t ImplBytes  = sizeof(void*);                               //!< implementation extra
        static const size_t UUID_Size  = ClassSize - (CXX_Bytes+ImplBytes+DepthSize); //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        explicit Lockable(const char *id) noexcept; //!< setup with optional id
    public:
        virtual ~Lockable() noexcept;

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        void             lock()    noexcept; //!< doLock and ++depth
        void             unlock()  noexcept; //!< --depth and doUnlock
        bool             tryLock() noexcept; //!< doTryLock and ++depth
        static Lockable &Giant();            //!< the top-level mutex

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        const  int  depth;           //!< bookeeping callas
        const  char uuid[UUID_Size]; //!< optional uuid

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Lockable);
        virtual void doLock()    noexcept = 0;
        virtual void doUnlock()  noexcept = 0;
        virtual bool doTryLock() noexcept = 0;
    };

    //! helper to derive a lockable class
#define Y_LOCKABLE_API()           \
virtual void doLock()    noexcept; \
virtual void doUnlock()  noexcept; \
virtual bool doTryLock() noexcept


    //__________________________________________________________________________
    //
    //
    //
    //! Scoped Lock
    //
    //
    //__________________________________________________________________________
    class ScopedLock
    {
    public:
        ScopedLock(Lockable &) noexcept; //!< setup by locking host
        ~ScopedLock()          noexcept; //!< unlock host
    private:
        Y_DISABLE_COPY_AND_ASSIGN(ScopedLock);
        Lockable &host;
    };

    //! create the guard name
#define Y_LOCK__(X,Y) X##Y

    //! instantiace the guard name
#define Y_LOCK_(HOST,ID) volatile Yttrium::ScopedLock Y_LOCK__(__guard,ID)(HOST)

    //! use a local ScopedLock to lock HOST
#define Y_LOCK(HOST) Y_LOCK_(HOST,__LINE__)

    //! declare and use the giant lock
#define Y_GIANT_LOCK() static Yttrium::Lockable &giantLock = Yttrium::Lockable::Giant();\
Y_LOCK(giantLock)
    
}

#endif

