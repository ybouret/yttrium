
//! \file

#ifndef Y_Concurrent_Alone_Included
#define Y_Concurrent_Alone_Included 1

#include "y/concurrent/pipeline/interface.hpp"
#include "y/concurrent/fake-lock.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Interface to single (main) thread pipeline
        //
        //
        //______________________________________________________________________
        class Alone : public Pipeline
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< Concurrent::Alone

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Alone(const char *id=0) noexcept; //!< setup with optional lock name
            virtual ~Alone() noexcept;                 //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual const char * callSign()               const noexcept; //!< CallSign
            virtual size_t       size()                   const noexcept; //!< 1
            virtual Lockable    &sync()                         noexcept; //!< mine
            virtual ConstType &  operator[](const size_t) const noexcept; //!< self
            virtual void         flush()                        noexcept; //!< do nothing...
            virtual TaskFlag     query(const TaskUUID)    const noexcept; //!< always success
            virtual size_t       cargo()                  const noexcept; //!< zero

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Alone);
            FakeLock      mine;
            ThreadContext self;

            virtual void     suspend() noexcept;
            virtual void     restart() noexcept;
            virtual TaskUUID enqueue(const Task &, const TaskUUID);
        };
    }

}

#endif

