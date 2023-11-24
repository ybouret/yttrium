
//! \file

#ifndef Y_Concurrent_Loop_Kernel_Included
#define Y_Concurrent_Loop_Kernel_Included 1

#include "y/concurrent/thread/context.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! interface to SYNCHRONOUS kernel for Loop
        //
        //
        //______________________________________________________________________
        class Kernel
        {
        public:
            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual void operator()(const ThreadContext &) = 0; //!< call

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Kernel() noexcept; //!< cleanup
        protected:
            explicit Kernel() noexcept; //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Kernel);
        };

    }

}

#endif
