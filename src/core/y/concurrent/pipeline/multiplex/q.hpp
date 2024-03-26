
//! \file

#ifndef Y_Concurrent_Pipeline_Multiplex_Queue_Included
#define Y_Concurrent_Pipeline_Multiplex_Queue_Included 1

#include "y/concurrent/pipeline/interface.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        class MultiplexQueue
        {
        public:
            Pipeline *        operator->()       noexcept;
            const Pipeline  * operator->() const noexcept;
            
            virtual ~MultiplexQueue()                       noexcept;
        protected:
            explicit MultiplexQueue(const SharedPipeline &) noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(MultiplexQueue);
            Pipeline * const Q;
        };

    }

}

#endif

