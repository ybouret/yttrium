//! \file

#ifndef Y_Concurrent_Runnable_Included
#define Y_Concurrent_Runnable_Included 1

#include "y/concurrent/thread/context.hpp"
#include "y/object.hpp"
#include "y/counted.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Runnable code for Pipeline
        //
        //
        //______________________________________________________________________
        class Runnable : public Object, public Counted
        {
        public:
            //__________________________________________________________________
            //
            //
            // interface
            //
            //__________________________________________________________________
            virtual void run(const ThreadContext &) = 0; //!< running code on a given context

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Runnable() noexcept; //!< cleanup
        protected:
            explicit Runnable() noexcept; //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Runnable);
        };
    }

}

#endif

