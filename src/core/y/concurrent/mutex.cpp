#include "y/concurrent/mutex.hpp"
#include "y/system/exception.hpp"
#include "y/system/error.hpp"
#include "y/system/at-exit.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/type/destruct.hpp"


#include <cstring>
#include <new>

#if defined(Y_BSD)
#    if !defined(__USE_UNIX98)
#        define __USE_UNIX98
#    endif
#    include <pthread.h>
typedef pthread_mutex_t MutexType;
#endif

#if defined(Y_WIN)
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
typedef CRITICAL_SECTION MutexType;
#endif

namespace Yttrium
{
    namespace Concurrent
    {

        namespace Quark
        {

#if defined(Y_BSD)
            class MutexAttribute
            {
            public:
                inline explicit MutexAttribute() : attr()
                {
                    {
                        const int  res = pthread_mutexattr_init(&attr);
                        if (res != 0) throw Libc::Exception(res, "pthread_mutexattr_init");
                    }

                    {
                        const int res = pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
                        if (res != 0)
                        {
                            pthread_mutexattr_destroy(&attr);
                            throw Libc::Exception(res, "pthread_mutexattr_settype(RECURSIVE)");
                        }
                    }
                }

                inline virtual ~MutexAttribute() noexcept
                {
                    (void) pthread_mutexattr_destroy(&attr);
                }

                pthread_mutexattr_t attr;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(MutexAttribute);
            };
#endif

#if         defined(Y_WIN)
            struct MutexAttribute {};
#endif

            class Mutex
            {
            public:
                inline Mutex(MutexAttribute &param) : m()
                {

#if                 defined(Y_BSD)
                    const int res = pthread_mutex_init(&m,&param.attr);
                    if( res != 0 ) throw Libc::Exception(res,"pthread_mutex_init");

#endif

#if                 defined(Y_WIN)
                    (void)param;
                    ::InitializeCriticalSection(&m);
#endif

                }

                inline ~Mutex() noexcept
                {
#if                 defined(Y_BSD)
                    const int res = pthread_mutex_destroy(&m);
                    if(res!=0) Libc::CriticalError(res,"pthread_mutex_destroy");

#endif

#if                 defined(Y_WIN)
                    ::DeleteCriticalSection(&m);
#endif
                }

                inline void lock() noexcept
                {
#if defined(Y_BSD)
                    const int res = pthread_mutex_lock(&m);
                    if( res != 0 ) Libc::CriticalError(res,"pthread_mutex_lock");
#endif

#if defined(Y_WIN)
                    ::EnterCriticalSection(&m);
#endif
                }

                inline void unlock() noexcept
                {
#if defined(Y_BSD)
                    const int res = pthread_mutex_unlock(&m);
                    if( res != 0 ) Libc::CriticalError(res,"pthread_mutex_unlock");
#endif

#if defined(Y_WIN)
                    ::LeaveCriticalSection(&m);
#endif
                }

                inline bool tryLock() noexcept
                {
#if defined(Y_WIN)
                    return ::TryEnterCriticalSection(&m) == TRUE;
#endif

#if defined(Y_BSD)
                    return pthread_mutex_trylock(&m) == 0;
#endif
                }

                MutexType m;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Mutex);
            };



            class Atelier : public Lockable
            {
            public:
                inline virtual ~Atelier() noexcept {}

                inline explicit Atelier() :
                Lockable("GIANT"),
                param(),
                giant(param)
                {
                }

                MutexAttribute param;
                Mutex          giant;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Atelier);
                inline virtual void doLock()     noexcept { giant.lock();   }
                inline virtual void doUnlock()   noexcept { giant.unlock(); }
                inline virtual bool doTryLock()  noexcept { return giant.tryLock(); }
            };

            static void    *Atelier__[ Y_WORDS_FOR(Atelier) ];
            static Atelier *Atelier_ = 0;
            static bool     AtelierInit = true;
            static void     AtelierQuit(void*) noexcept
            {
                if(Atelier_)
                {
                    Memory::OutOfReach::Zero( Destructed(Atelier_), sizeof(Atelier_) );
                    Atelier_ = 0;
                }
            }

            static Atelier & AtelierInstance()
            {
                if(!Atelier_)
                {
                    if(AtelierInit)
                    {
                        AtExit::Register(AtelierQuit, 0, AtExit::MaximumLongevity);
                        AtelierInit = false;
                    }

                    try {
                        Atelier_ = new ( Y_STATIC_ZARR(Atelier__) ) Atelier();
                    }
                    catch(...)
                    {
                        Atelier_ = 0;
                        throw;
                    }

                }
                assert(0!=Atelier_);
                return *Atelier_;
            }



        }

    }

}


namespace Yttrium
{
    namespace Concurrent
    {

        Lockable & Mutex::Giant()
        {
            static Lockable &giant = Quark::AtelierInstance();
            return giant;
        }

        Mutex:: Mutex(const char *id) :
        Lockable(id),
        mutex(0)
        {
        }

        Mutex:: ~Mutex() noexcept
        {
            
        }

        void Mutex:: doLock() noexcept {
            mutex->lock();
        }

        void Mutex:: doUnlock() noexcept {
            mutex->unlock();
        }

        bool Mutex:: doTryLock() noexcept {
            return mutex->tryLock();
        }
    }
}

