#include "y/concurrent/mutex.hpp"
#include "y/system/exception.hpp"
#include "y/system/error.hpp"
#include "y/system/at-exit.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/type/destruct.hpp"
#include "y/memory/album.hpp"
#include "y/memory/guild.hpp"

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

#include "mutex.hxx"


namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {
            //------------------------------------------------------------------
            //
            //
            // Lowest possible singleton to provide a giant mutex and
            // concurrent data
            //
            //------------------------------------------------------------------
            class Quark : public Lockable
            {
            public:
                inline virtual ~Quark() noexcept {}

                inline explicit Quark() :
                Lockable("GIANT"),
                param(),
                giant(param),
                album(*this),
                mutexes(album, Memory::Page::DefaultBytes)
                {
                }

                inline Mutex *createMutex()
                {
                    return mutexes.construct(param);
                }

                inline void deleteMutex(Mutex * &mutex) noexcept
                {
                    mutexes.eradicate(mutex);
                }


                MutexAttribute       param;
                Mutex                giant;
                Memory::Album        album;
                Memory::Guild<Mutex> mutexes;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Quark);
                inline virtual void doLock()     noexcept { giant.lock();   }
                inline virtual void doUnlock()   noexcept { giant.unlock(); }
                inline virtual bool doTryLock()  noexcept { return giant.tryLock(); }
            };

            static void     *Quark__[ Y_WORDS_FOR(Quark) ];
            static Quark    *Quark_ = 0;
            static bool      QuarkInit = true;
            static void      QuarkQuit(void*) noexcept
            {
                if(Quark_)
                {
                    Memory::OutOfReach::Zero( Destructed(Quark_), sizeof(Quark__) );
                    Quark_ = 0;
                }
            }

            static Quark & QuarkInstance()
            {
                if(!Quark_)
                {
                    std::cerr << "sizeof(Quark) = " << sizeof(Quark) << "/" << sizeof(Quark__) << std::endl;
                    std::cerr << "sizeof(Mutex) = " << sizeof(Mutex) << std::endl;

                    if(QuarkInit)
                    {
                        AtExit::Register(QuarkQuit, 0, AtExit::MaximumLongevity);
                        QuarkInit = false;
                    }

                    try
                    {
                        Quark_ = new ( Y_STATIC_ZARR(Quark__) ) Quark();
                    }
                    catch(...)
                    {
                        Quark_ = 0;
                        throw;
                    }

                }
                
                assert(0!=Quark_);
                return *Quark_;
            }



        }

    }

}


namespace Yttrium
{

    Lockable & Lockable::Giant()
    {
        static Lockable &giant = Concurrent:: Nucleus:: QuarkInstance();
        return giant;
    }

    namespace Concurrent
    {
        
        Mutex:: Mutex(const char *id) :
        Lockable(id),
        quark( Nucleus::QuarkInstance() ),
        mutex( quark.createMutex() )
        {
        }

        Mutex:: ~Mutex() noexcept
        {
            assert(0!=mutex);
            quark.deleteMutex(mutex);
            assert(0==mutex);
        }

        void Mutex:: doLock() noexcept {
            assert(0!=mutex);
            mutex->lock();
        }

        void Mutex:: doUnlock() noexcept {
            assert(0!=mutex);
            mutex->unlock();
        }

        bool Mutex:: doTryLock() noexcept {
            assert(0!=mutex);
            return mutex->tryLock();
        }
    }
}

