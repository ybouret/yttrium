
//! \file

#ifndef Y_Thread_Named_Included
#define Y_Thread_Named_Included 1

#include "y/concurrent/thread.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        class NamedThread : public Thread
        {
        public:
            explicit NamedThread(ThreadProc proc, void *args);
            virtual ~NamedThread() noexcept;
            
            const ThreadHandle name;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(NamedThread);
        };

    }

}

#endif
