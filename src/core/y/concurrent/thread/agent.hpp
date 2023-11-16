
//! \file

#ifndef Y_Thread_Agent_Included
#define Y_Thread_Agent_Included 1

#include "y/concurrent/thread/context.hpp"
#include "y/concurrent/wire.hpp"
#include "y/functor.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        class Threads;

        typedef Functor<void,TL1(const ThreadContext&)> Kernel;

        class Agent : public ThreadContext, public Wire
        {
        public:
            explicit Agent(const size_t sz, const size_t rk, Lockable &mx, Threads &);
            virtual ~Agent() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Agent);
        };

    }

}
#endif
