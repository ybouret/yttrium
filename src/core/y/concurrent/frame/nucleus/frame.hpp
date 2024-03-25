
//! \file

#ifndef Y_CONCURRENT_FRAME_NUCLEUS_INCLUDED
#define Y_CONCURRENT_FRAME_NUCLEUS_INCLUDED 1

#include "y/concurrent/thread/context.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        namespace Nucleus
        {
            //__________________________________________________________________
            //
            //
            //
            //! base class for a Frame
            //
            //
            //__________________________________________________________________
            class Frame : public ThreadContext
            {
            public:
                virtual ~Frame() noexcept; //!< cleanup

            protected:
                explicit Frame(const ThreadContext &) noexcept; //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Frame);
            };
        }

    }

}

#endif
