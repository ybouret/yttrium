
//! \file

#ifndef Y_CONCURRENT_FRAME_INCLUDED
#define Y_CONCURRENT_FRAME_INCLUDED 1

#include "y/concurrent/thread/context.hpp"
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
        class Frame : public ThreadContext
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef MAPPING Mapping; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            //! setup, copy context, empty workspace
            inline explicit Frame(const ThreadContext &ctx) noexcept :
            ThreadContext(ctx), workspace() {}

        public:
            //! cleanup
            inline virtual ~Frame() noexcept {}

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
           
            
            virtual void shutdown() noexcept
            {

            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! check that assignation was performed
            inline bool isAssigned() const noexcept { return workspace.isValid(); }

            //! release workspace
            inline void loosen() noexcept {
                this->workspace.erase();
                this->shutdown();
            }



            //! display content
            inline friend std::ostream & operator<<(std::ostream &os, const Frame &frame)
            {
                os << "@" << frame.name << "->" << frame.workspace;
                return os;
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
        protected:
            Memory::Workspace<MAPPING> workspace; //!< blank space/content

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frame);
        };
    }

}

#endif
