//! \file

#ifndef Y_Concurrent_Runnable_Included
#define Y_Concurrent_Runnable_Included 1

#include "y/concurrent/thread/context.hpp"
#include "y/object.hpp"
#include "y/counted.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        class Runnable : public Object, public Counted
        {
        public:
            virtual void run(const ThreadContext &) = 0;

            virtual ~Runnable() noexcept;
        protected:
            explicit Runnable() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Runnable);
        };
    }

}

#endif

