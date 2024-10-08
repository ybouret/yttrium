//! \file


#ifndef  Y_Singleton_Included
#define  Y_Singleton_Included 1

#include "y/concurrent/singulet.hpp"
#include "y/locking/regular.hpp"

#include "y/memory/out-of-reach.hpp"
#include "y/calculus/align.hpp"

#include <iostream>

namespace Yttrium
{
    
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
    template <typename T, class POLICY = Locking::Regular >
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

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        inline virtual const char *             callSign() const noexcept { return T::CallSign; } //!< forward CallSign
        inline virtual AtExit::Longevity        lifeTime() const noexcept { return T::LifeTime; } //!< forward LifeTime
        MutexType                                access;                                          //!< class/system level mutex

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
            Y_GIANT_LOCK();
            if(0 == Instance_)
            {
                Y_GIANT_LOCK();
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

        //! setup
        inline explicit Singleton() noexcept :
        Concurrent::Singulet(),
        access(T::CallSign)
        {}

        //! cleanup
        inline virtual ~Singleton() noexcept {}


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

    template <typename T,class R> bool   Singleton<T,R>:: Register  = true;
    template <typename T,class R> T *    Singleton<T,R>:: Instance_ = 0;
    template <typename T,class R> size_t Singleton<T,R>:: Required  = 0;




}

#endif

