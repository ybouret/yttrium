
//! \file

#ifndef Y_Concurrent_Pipeline_Multiplex_Queue_Included
#define Y_Concurrent_Pipeline_Multiplex_Queue_Included 1

#include "y/concurrent/pipeline/interface.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Reference to a Pipeline/Queue
        //
        //
        //______________________________________________________________________
        class MultiplexQueue
        {
        public:
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Pipeline *        operator->()       noexcept; //!< access
            const Pipeline  * operator->() const noexcept; //!< access

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~MultiplexQueue()                       noexcept; //!< cleanup
        protected:
            explicit MultiplexQueue(const SharedPipeline &) noexcept; //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MultiplexQueue);
            Pipeline * const Q;
        };

    }

}

#endif

