//! \file

#ifndef Y_Thread_Included
#define Y_Thread_Included 1

#include "y/concurrent/primitive.hpp"
#include "y/concurrent/thread-proc.hpp"

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
            explicit Thread(ThreadProc proc, void *args); //!< launch proc(args)
            virtual ~Thread() noexcept;                   //!< join

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Thread);
            Nucleus::Thread *thread;
        };
    }
}

#endif
