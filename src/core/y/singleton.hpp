//! \file


#ifndef  Y_Singleton_Included
#define  Y_Singleton_Included 1

#include "y/concurrent/singulet.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/calculus/align.hpp"
#include "y/type/pick.hpp"

#include <iostream>

namespace Yttrium
{

    class NucleusSingleton
    {
    public:
        static const bool                IsRegular = false;
        static const bool                IsNucleus = true;
        typedef Concurrent::NucleusMutex MutexType;
    };

    class RegularSingleton
    {
    public:
        static const bool                IsRegular = true;
        static const bool                IsNucleus = false;
        typedef Concurrent::Mutex        MutexType;
    };


    //__________________________________________________________________________
    //
    //
    //
    // Building Singleton for any class
    /**
     assuming that class have
     - a 'const char * const CallSign'
     - 'AtExit::Longevity  LifeTime'
     */
    //
    //
    //__________________________________________________________________________
    template <typename T, class POLICY = RegularSingleton >
    class Singleton : public Concurrent::Singulet, public POLICY
    {
    public:
        typedef typename POLICY::MutexType MutexType;
        typedef Singleton<T,POLICY>        SingletonType;

        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static MutexType Access; //!< static class level mutex

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        inline virtual const char *      callSign() const noexcept { return T::CallSign; } //!< forward CallSign
        inline virtual AtExit::Longevity lifeTime() const noexcept { return T::LifeTime; } //!< forward LifeTime

        //______________________________________________________________________
        //
        //! return or create Instance
        /**
         - register destruction upon first call
         - create if not
         */
        //______________________________________________________________________
        static inline T & Instance()
        {
            static void * Instance__[ Y_WORDS_FOR(T) ] = { 0 };
            Y_LOCK(Access);
            if(0 == Instance_)
            {
                Y_LOCK(Access);
                if(Register) {
                    CheckLifeTime(T::CallSign,T::LifeTime);
                    AtExit::Register(Quit, 0, T::LifeTime);
                    Register = false;
                    Required = sizeof(Instance__);
                }

                return *(Instance_ = new ( Y_STATIC_ZARR(Instance__) ) T() );
            }
            else
                return *Instance_;
        }

        //______________________________________________________________________
        //
        //! return checked existing Instance
        //______________________________________________________________________
        static inline T &Location() noexcept
        {
            CheckInstance(T::CallSign,Instance_);
            assert(Instance_);
            return *Instance_;
        }


    protected:
        inline explicit Singleton() noexcept : Concurrent::Singulet() {} //!< setup
        inline virtual ~Singleton() noexcept {}                          //!< cleanup


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Singleton);

        static T    *       Instance_;
        static bool         Register;
        static size_t       Required;

        static inline void  Quit(void*) noexcept {
            if(0!=Instance_) {
                Instance_->~T();
                Memory::OutOfReach::Zero(Instance_,Required);
                Instance_ = 0;
            }
        }
    };

    template <typename T,class R> bool   Singleton<T,R>:: Register  = true;
    template <typename T,class R> T *    Singleton<T,R>:: Instance_ = 0;
    template <typename T,class R> size_t Singleton<T,R>:: Required  = 0;

    template <typename T,class R>
    typename Singleton<T,R>::MutexType Singleton<T,R>:: Access(T::CallSign);


}

#endif

