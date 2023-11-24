//! \file

#ifndef Y_Concurrent_Job_Included
#define Y_Concurrent_Job_Included 1

#include "y/concurrent/runnable.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        class Job : public ArcPtr<Runnable>
        {
        public:
            explicit Job(Runnable *) noexcept;
            virtual ~Job() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Job);
        };
    }
}

#endif

