//! \file

#ifndef Y_Concurrent_Loop_Included
#define Y_Concurrent_Loop_Included 1

#include "y/concurrent/thread/context.hpp"
#include "y/container/readable.hpp"
#include "y/functor.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! a kernel to run within a Thread
        //
        //
        //______________________________________________________________________
        typedef Functor<void,TL1(const ThreadContext&)> ThreadKernel;


        //______________________________________________________________________
        //
        //
        //
        //! Interface to Loop a kernel in single/multiple thread
        //
        //
        //______________________________________________________________________
        class Loop : public Readable<const ThreadContext>
        {
        public:
            //! execute the kernel, return when all done
            virtual void operator()(ThreadKernel &) noexcept = 0;


            virtual ~Loop() noexcept; //!< cleanup
        protected:
            explicit Loop() noexcept; //!< setup
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Loop);
        };

    }

}

#endif

