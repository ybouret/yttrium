//! \file


#ifndef  Y_Singleton_Included
#define  Y_Singleton_Included 1

#include "y/concurrent/singulet.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/calculus/align.hpp"

#include <iostream>

namespace Yttrium
{

    template <typename T>
    class Singleton : public Concurrent::Singulet
    {
    public:
        static Concurrent::Mutex Access;

        inline virtual const char *      callSign() const noexcept { return T::CallSign; }
        inline virtual AtExit::Longevity lifeTime() const noexcept { return T::LifeTime; }
        
        static inline T & Instance()
        {
            Y_LOCK(Access);
            if(0 == Instance_)
            {
                Y_LOCK(Access);
                if(Register)
                {
                    AtExit::Register(Quit,0, T::LifeTime);
                    Register = false;
                }

                Instance_ = new ( Memory::OutOfReach::Zero(Instance__,Required) ) T();
            }

            assert(0!=Instance_);
            return *Instance_;
        }

    protected:
        inline explicit Singleton() noexcept : Concurrent::Singulet() {}
        inline virtual ~Singleton() noexcept {}


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

