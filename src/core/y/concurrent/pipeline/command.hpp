


//! \file

#ifndef Y_Concurrent_Command_Included
#define Y_Concurrent_Command_Included 1

#include "y/concurrent/pipeline/runnable.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        template <typename OBJECT, typename METHOD>
        class Command : public Runnable
        {
        public:
            inline explicit Command(OBJECT &o, METHOD m) noexcept : Runnable(), host(o), meth(m) {}
            inline virtual ~Command() noexcept {}

            inline virtual void run(const ThreadContext &context) { (host.*meth)(context); }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Command);
            OBJECT &host;
            METHOD  meth;
        };

    }

}

#endif

