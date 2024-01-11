
//! \file

#ifndef Y_Concurrent_Callback0_Included
#define Y_Concurrent_Callback0_Included 1

#include "y/concurrent/pipeline/runnable.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //! function or functionoid, using copy/copy constructor
        //
        //______________________________________________________________________
        template <typename FUNCTION>
        class Callback0 : public Runnable
        {
        public:
            inline explicit Callback0(const FUNCTION &fn) : Runnable(), function(fn) {} //!< setup
            inline virtual ~Callback0() noexcept {}                                     //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Callback0);
            inline virtual void run(const ThreadContext &context) { function(context); }

            FUNCTION function;
        };

    }

}

#endif


