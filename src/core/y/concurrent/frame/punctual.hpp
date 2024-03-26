//! \file

#ifndef Y_CONCURRENT_PUNCTUAL_FRAME_INCLUDED
#define Y_CONCURRENT_PUNCTUAL_FRAME_INCLUDED 1

#include "y/concurrent/frame/nucleus/punctual.hpp"
#include "y/concurrent/frame/interface.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! Frame for punctual operations
        //
        //
        //______________________________________________________________________
        class PunctualFrame : public Frame<Nucleus::Punctual>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef int               Type;       //!< N/A
            typedef Nucleus::Punctual Mapping;    //!< alias
            typedef Frame<Mapping>    FrameType;  //!< alias

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! mark workspace as built
            inline void attach() { this->workspace.build(); }

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! cleanup
            inline virtual ~PunctualFrame() noexcept {}

        protected:
            //! setup with context and empty workspace
            inline explicit PunctualFrame(const ThreadContext &ctx) noexcept :
            FrameType(ctx) {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PunctualFrame);

        };

    }
}

#endif

