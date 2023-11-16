
//! \file

#ifndef Y_Concurrent_Thread_Context_Included
#define Y_Concurrent_Thread_Context_Included 1

#include "y/concurrent/context.hpp"
#include "y/lockable.hpp"


namespace Yttrium
{

    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! Context for Thread, with synchronization reference
        //
        //
        //______________________________________________________________________
        class ThreadContext : public Context
        {
        public:
            explicit ThreadContext(const size_t sz, const size_t rk, Lockable &mx) noexcept; //!< setup
            explicit ThreadContext(const ThreadContext &) noexcept;                          //!< copy
            virtual ~ThreadContext() noexcept;                                               //!< cleanup

            Lockable &sync; //!< shared sync object reference

        private:
            Y_DISABLE_ASSIGN(ThreadContext);
        };
    }

}


#endif

