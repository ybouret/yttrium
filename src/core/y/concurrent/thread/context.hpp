
//! \file

#ifndef Y_Concurrent_Thread_Context_Included
#define Y_Concurrent_Thrad_Context_Included 1

#include "y/concurrent/context.hpp"
#include "y/lockable.hpp"


namespace Yttrium
{

    namespace Concurrent
    {

        class ThreadContext : public Context
        {
        public:
            explicit ThreadContext(const size_t sz, const size_t rk, Lockable &mx) noexcept;
            explicit ThreadContext(const ThreadContext &) noexcept;
            explicit ThreadContext(Lockable &) noexcept;
            virtual ~ThreadContext() noexcept;

            Lockable &sync;

        private:
            Y_DISABLE_ASSIGN(ThreadContext);
        };
    }

}


#endif

