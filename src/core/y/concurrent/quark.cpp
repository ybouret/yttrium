#include "y/concurrent/mutex.hpp"
#include "y/system/exception.hpp"
#include "y/system/error.hpp"
#include "y/memory/out-of-reach.hpp"



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

#include "y/system/at-exit.hpp"
#include "y/calculus/align.hpp"
#include "y/type/destruct.hpp"
#include "y/memory/album.hpp"
#include "y/memory/guild.hpp"
#include "y/memory/blocks.hpp"
#include "y/memory/straps.hpp"
#include "y/memory/quanta.hpp"
#include "y/memory/corpus.hpp"
#include "y/concurrent/singulet.hpp"

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
            class Quark : public Lockable, public Singulet
            {
            public:
                static const char * const       CallSign;
                static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity;

                inline virtual ~Quark() noexcept {}

                inline explicit Quark() :
                Lockable("GiantMutex"),
                param(),
                giant(param),
                album(*this),
                mutexes(album),
                blocks(album),
                straps(album),
                quanta(blocks,straps),
                corpus(album)
                {
                }

                inline Mutex *createMutex()
                {
                    return mutexes.construct(param);
                }

                inline void deleteMutex(Mutex * &mutex) noexcept
                {
                    mutexes.eradicate(mutex);
                    mutex = 0;
                }

                virtual const char *       callSign() const noexcept { return CallSign; }
                virtual AtExit::Longevity  lifeTime() const noexcept { return LifeTime; }


                MutexAttribute       param;
                Mutex                giant;
                Memory::Album        album;
                Memory::Guild<Mutex> mutexes;
                Memory::Blocks       blocks;
                Memory::Straps       straps;
                Memory::Quanta       quanta;
                Memory::Corpus       corpus;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Quark);
                inline virtual void doLock()     noexcept { giant.lock();   }
                inline virtual void doUnlock()   noexcept { giant.unlock(); }
                inline virtual bool doTryLock()  noexcept { return giant.tryLock(); }
            };

            const char * const Quark:: CallSign = "Quark";

            static void     *Quark__[ Y_WORDS_FOR(Quark) ];
            static Quark    *Quark_ = 0;
            static bool      QuarkInit = true;
            static void      QuarkQuit(void*) noexcept
            {
                if(Quark_)
                {
                    Singulet::OnQuitDisplay(Quark::CallSign,Quark::LifeTime);
                    Memory::OutOfReach::Zero( Destructed(Quark_), sizeof(Quark__) );
                    Quark_ = 0;
                }
            }

            static Quark & QuarkInstance()
            {
                if(!Quark_)
                {
                    
                    if(QuarkInit)
                    {
                        Singulet::OnInitDisplay(Quark::CallSign,Quark::LifeTime);
                        AtExit::Register(QuarkQuit, 0, Quark::LifeTime);
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
        MutexProto:: ~MutexProto() noexcept
        {
        }

        MutexProto:: MutexProto(const char *id) :
        Lockable(id),
        quark( Nucleus::QuarkInstance() )
        {
        }

    }

    namespace Concurrent
    {
        NucleusMutex:: NucleusMutex(const char *id) : MutexProto(id) {}
        NucleusMutex:: ~NucleusMutex() noexcept {}

        void NucleusMutex:: doLock() noexcept {
            quark.lock();
        }

        void NucleusMutex:: doUnlock() noexcept {
            quark.unlock();
        }

        bool NucleusMutex:: doTryLock() noexcept {
            return quark.tryLock();
        }

        Nucleus::Mutex & NucleusMutex:: operator*() noexcept
        {
            return quark.giant;
        }
    }

    namespace Concurrent
    {

        Mutex:: Mutex(const char *id) :
        MutexProto(id),
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

        Nucleus::Mutex & Mutex:: operator*() noexcept
        {
            assert(0!=mutex);
            return *mutex;
        }
    }
}

#include "y/concurrent/memory.hpp"



namespace Yttrium
{
    namespace Concurrent
    {

        Memory::Straps & Mem:: StrapsInstance()
        {
            return Nucleus:: QuarkInstance().straps;
        }

        Memory::Corpus & Mem:: CorpusInstance()
        {
            return Nucleus:: QuarkInstance().corpus;
        }

        Memory::Quanta & Mem:: QuantaInstance()
        {
            return Nucleus:: QuarkInstance().quanta;
        }


        Memory::Blocks & Mem:: BlocksInstance()
        {
            return Nucleus:: QuarkInstance().blocks;
        }
    }

}
