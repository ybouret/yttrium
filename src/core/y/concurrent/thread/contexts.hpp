
//! \file

#ifndef Y_Concurrent_Thread_Contexts_Included
#define Y_Concurrent_Thread_Contexts_Included 1

#include "y/concurrent/thread/context.hpp"
#include "y/object.hpp"
#include "y/counted.hpp"
#include "y/container/readable.hpp"

namespace Yttrium
{

    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! base class for a group of contexts
        //
        //
        //______________________________________________________________________
        class ThreadContexts :
        public Object,
        public Counted,
        public Readable<const ThreadContext>
        {
        public:
            virtual ~ThreadContexts() noexcept; //!< cleanup

        protected:
            explicit ThreadContexts() noexcept; //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ThreadContexts);
        };

    }

}

#endif
