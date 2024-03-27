
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
        class MultiplexPipeline
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
            virtual ~MultiplexPipeline()                       noexcept; //!< cleanup
        protected:
            explicit MultiplexPipeline(const SharedPipeline &) noexcept; //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MultiplexPipeline);
            Pipeline * const Q;
        };

    }

}

#endif

