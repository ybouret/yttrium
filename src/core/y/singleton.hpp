//! \file


#ifndef  Y_Singleton_Included
#define  Y_Singleton_Included 1

#include "y/concurrent/singulet.hpp"
#include "y/concurrent/mutex.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/calculus/align.hpp"

#include <iostream>

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Base class to use Singleton based on system wide mutex
    //
    //
    //__________________________________________________________________________
    class NucleusSingleton
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const bool                IsRegular = false; //!< alias
        static const bool                IsNucleus = true;  //!< alias
        typedef Concurrent::NucleusMutex MutexType;         //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit NucleusSingleton() noexcept;
        virtual ~NucleusSingleton() noexcept;
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(NucleusSingleton);
    };


    //__________________________________________________________________________
    //
    //
    //
    //! Base class to use Singleton based on class wide mutex
    //
    //
    //__________________________________________________________________________
    class RegularSingleton
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const bool                IsRegular = true;  //!< alias
        static const bool                IsNucleus = false; //!< alias
        typedef Concurrent::Mutex        MutexType;         //!< alias

        explicit RegularSingleton() noexcept;
        virtual ~RegularSingleton() noexcept;
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(RegularSingleton);
    };


    //__________________________________________________________________________
    //
    //
    //
    // Building Singleton for any class
    /**
     assuming that class have
     - a 'const char * const CallSign'
     - a 'AtExit::Longevity  LifeTime'
     */
    //
    //
    //__________________________________________________________________________
    template <typename T, class POLICY = RegularSingleton >
    class Singleton : public Concurrent::Singulet, public POLICY
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef typename POLICY::MutexType MutexType;     //!< alias
        typedef Singleton<T,POLICY>        SingletonType; //!< alias
        static MutexType                   Access;        //!< static class/system level mutex

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        inline virtual const char *             callSign() const noexcept { return T::CallSign; } //!< forward CallSign
        inline virtual AtExit::Longevity        lifeTime() const noexcept { return T::LifeTime; } //!< forward LifeTime

        //______________________________________________________________________
        //
        //! check existence
        //______________________________________________________________________
        static bool Exists() noexcept { return 0 != Instance_; }

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
                    OnInitDisplay(T::CallSign,T::LifeTime);
                    CheckLifeTime(T::CallSign,T::LifeTime);
                    AtExit::Register(Quit, 0, T::LifeTime);
                    Register = false;
                    Required = sizeof(Instance__);
                }

                return *( Instance_ = new ( Y_STATIC_ZARR(Instance__) ) T() );
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
                OnQuitDisplay(T::CallSign,T::LifeTime);
                Instance_->~T();
                Memory::OutOfReach::Zero(Instance_,Required);
                Instance_ = 0;
            }
        }
    };

    template <typename T,class R>
    typename Singleton<T,R>::MutexType   Singleton<T,R>:: Access(T::CallSign);
    template <typename T,class R> bool   Singleton<T,R>:: Register  = true;
    template <typename T,class R> T *    Singleton<T,R>:: Instance_ = 0;
    template <typename T,class R> size_t Singleton<T,R>:: Required  = 0;




}

#endif

