//! \file

#ifndef Y_Concurrent_Threads_Included
#define Y_Concurrent_Threads_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        class Thread;

        //______________________________________________________________________
        //
        //
        //
        //! basic interface to access individual threads
        //
        //
        //______________________________________________________________________
        class Threads
        {
        public:
            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual size_t         numThreads()            const noexcept = 0; //!< inner number of threads
            virtual const Thread & getThread(const size_t) const noexcept = 0; //!< get thread in [1..numThreads()]

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Threads() noexcept; //!< cleanup
        protected:
            explicit Threads() noexcept; //!< setup
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Threads);
        };

    }
}

#endif

