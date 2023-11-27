//! \file

#ifndef Y_Thread_Included
#define Y_Thread_Included 1

#include "y/concurrent/primitive.hpp"
#include "y/concurrent/thread/proc.hpp"
#include "y/concurrent/thread/handle.hpp"
#include "y/lockable.hpp"
#include <iosfwd>

namespace Yttrium
{
    namespace Concurrent
    {
        namespace Nucleus
        {
            class Thread;
        }

        //______________________________________________________________________
        //
        //
        //
        //! Basic thread wrapper
        //
        //
        //______________________________________________________________________
        class Thread : public Primitive
        {
        public:
            static bool          Verbose; //!< global verbosity if needed
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Thread(ThreadProc proc, void *args); //!< launch proc(args)
            virtual ~Thread() noexcept;                   //!< join

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void                  assign(const size_t j);          //!< best effort thread placement
            static ThreadHandle   CurrentHandle()        noexcept; //!< Human Readadble Thread Handle

            //__________________________________________________________________
            //
            //
            // members
            //
            //__________________________________________________________________
            const ThreadHandle handle; //!< system handle

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Thread);
            Nucleus::Thread *thread;
        };
    }
}

//! helper for verbose output within threads
#define Y_THREAD_MSG(MSG) \
do { if(Yttrium::Concurrent::Thread::Verbose) { Y_GIANT_LOCK(); (std::cerr << MSG << std::endl).flush(); } } while(false)

#endif
