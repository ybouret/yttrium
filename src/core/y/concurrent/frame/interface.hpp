
//! \file

#ifndef Y_CONCURRENT_FRAME_INCLUDED
#define Y_CONCURRENT_FRAME_INCLUDED 1

#include "y/concurrent/frame/nucleus/frame.hpp"
#include "y/memory/solitary/workspace.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Frame with assignable mapping
        /**
         MAPPING = [Nucleus::Punctual|ForLoop|AutoPtr Tile]
         */
        //
        //______________________________________________________________________
        template <typename MAPPING>
        class Frame : public Nucleus::Frame
        {
        protected:
            inline explicit Frame(const ThreadContext &ctx) noexcept :
            Nucleus::Frame(ctx), workspace() {}

        public:
            inline virtual ~Frame() noexcept { quit(); }

            
            inline void quit() noexcept { this->workspace.erase(); }

            inline friend std::ostream & operator<<(std::ostream &os, const Frame &frame)
            {
                os << "@" << frame.name << "->" << frame.workspace;
                return os;
            }

        protected:
            Memory::Workspace<MAPPING> workspace;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frame);
        };
    }

}

#endif
