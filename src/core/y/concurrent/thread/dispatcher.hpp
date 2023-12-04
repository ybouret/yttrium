

//! \file

#ifndef Y_Concurrent_Thread_Dispatcher_Included
#define Y_Concurrent_Thread_Dispatcher_Included 1

#include "y/concurrent/thread/contexts.hpp"
#include "y/ptr/arc.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{

    namespace Concurrent
    {

        typedef ArcPtr<ThreadContexts> SharedThreadContexts;

        class ThreadDispatcher 
        {
        public:
            virtual ~ThreadDispatcher() noexcept;
            explicit ThreadDispatcher(const SharedThreadContexts &) noexcept;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ThreadDispatcher);
        };


    }

}

#endif
