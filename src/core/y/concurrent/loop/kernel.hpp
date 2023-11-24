
//! \file

#ifndef Y_Concurrent_Loop_Kernel_Included
#define Y_Concurrent_Loop_Kernel_Included 1

#include "y/concurrent/thread/context.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        class Kernel
        {
        public:
            virtual void operator()(const ThreadContext &) = 0;

            virtual ~Kernel() noexcept;
        protected:
            explicit Kernel() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Kernel);
        };

    }

}

#endif
