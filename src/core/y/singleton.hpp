//! \file


#ifndef  Y_Singleton_Included
#define  Y_Singleton_Included 1

#include "y/concurrent/singulet.hpp"
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
     - 'AtExit::Longevity  LifeTime'
     */
    //
    //
    //__________________________________________________________________________
    template <typename T>
    class Singleton : public Concurrent::Singulet
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static Concurrent::Mutex Access; //!< static class level mutex

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
            Y_LOCK(Access);
            if(0 == Instance_)
            {
                Y_LOCK(Access);
                if(Register)
                {
                    CheckLifeTime(T::CallSign,T::LifeTime);
                    AtExit::Register(Quit, 0, T::LifeTime);
                    Register = false;
                }

                return *(Instance_ = new ( Memory::OutOfReach::Zero(Instance__,Required) ) T());
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
        static void *       Instance__[];
        static const size_t Required;
        static bool         Register;

        static inline void  Quit(void*) noexcept {
            if(0!=Instance_) {
                Instance_->~T();
                Memory::OutOfReach::Zero(Instance__,Required);
                Instance_ = 0;
            }
        }
    };

    template <typename T>
    bool Singleton<T>:: Register = true;

    template <typename T>
    T * Singleton<T>:: Instance_ = 0;

    template <typename T>
    void * Singleton<T>:: Instance__[ Y_WORDS_FOR(T) ];

    template <typename T>
    const size_t Singleton<T>:: Required = sizeof(Instance__);


    template <typename T>
    Concurrent::Mutex Singleton<T>:: Access(T::CallSign);
    

}

#endif

