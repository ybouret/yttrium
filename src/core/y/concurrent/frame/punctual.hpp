//! \file

#ifndef Y_CONCURRENT_PUNCTUAL_FRAME_INCLUDED
#define Y_CONCURRENT_PUNCTUAL_FRAME_INCLUDED 1

#include "y/concurrent/frame/nucleus/punctual.hpp"
#include "y/concurrent/frame/interface.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        class PunctualFrame : public Frame<Nucleus::Punctual>
        {
        public:
            typedef int               Type;       //!< N/A
            typedef Nucleus::Punctual Mapping;    //!< alias
            typedef Frame<Mapping>    FrameType;  //!< alias

            inline virtual ~PunctualFrame() noexcept {}

            inline void init() { this->workspace.build(); }


        protected:
            inline explicit PunctualFrame(const ThreadContext &ctx) noexcept :
            FrameType(ctx) {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PunctualFrame);

        };

    }
}

#endif

