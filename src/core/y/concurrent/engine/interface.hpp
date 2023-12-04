
//! \file

#ifndef Y_Concurrent_Engine_Included
#define Y_Concurrent_Engine_Included 1

#include "y/concurrent/thread/context.hpp"


namespace Yttrium
{
    namespace Concurrent
    {
     
        namespace Nucleus
        {
            class Engine
            {
            public:
                virtual void start(const ThreadContext &) = 0;
                
                virtual ~Engine() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Engine);
            };
        }



    }

}

#endif

