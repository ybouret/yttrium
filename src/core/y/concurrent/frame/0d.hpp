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
        class Frame0D : public Frame<Nucleus::Punctual>
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
            inline void assign(void) {
               (void) this->workspace.build();
            }

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! cleanup
            inline virtual ~Frame0D() noexcept {}

        protected:
            //! setup with context and empty workspace
            inline explicit Frame0D(const ThreadContext &ctx) noexcept :
            FrameType(ctx) {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frame0D);
            inline virtual void shutdown() noexcept {}


        };

    }
}

#endif

