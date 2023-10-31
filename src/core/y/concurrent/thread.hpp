//! \file

#ifndef Y_Thread_Included
#define Y_Thread_Included 1

#include "y/concurrent/primitive.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        namespace Nucleus
        {
            class Thread;
        }

        class Thread : public Primitive
        {
        public:
            explicit Thread();
            virtual ~Thread() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Thread);
            Nucleus::Thread *thread;
        };
    }
}

#endif
