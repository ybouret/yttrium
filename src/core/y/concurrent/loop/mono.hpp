
//! \file

#ifndef Y_Concurrent_Mono_Included
#define Y_Concurrent_Mono_Included 1

#include "y/concurrent/loop/interface.hpp"
#include "y/concurrent/fake-lock.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! Interface to single (main) thread loop
        //
        //
        //______________________________________________________________________
        class Mono : public Loop
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< Concurrent::Mono

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Mono(const char *id=0) noexcept; //!< setup with optional lock name
            virtual ~Mono() noexcept;                 //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual const char * callSign()               const noexcept; //!< CallSign
            virtual size_t       size()                   const noexcept; //!< 1
            virtual ConstType &  operator[](const size_t) const noexcept; //!< self
            virtual void         run1(Kernel &)                 noexcept; //!< direct call
            virtual Lockable &   sync()                         noexcept; //!< shared sync

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mono);

            FakeLock      mock;
            ThreadContext self;
        };

    }

}

#endif
