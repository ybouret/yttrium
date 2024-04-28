//! \file

#ifndef Y_CONCURRENT_NUCLEUS_FRAMES_INCLUDED
#define Y_CONCURRENT_NUCLEUS_FRAMES_INCLUDED 1

#include "y/ptr/arc.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/concurrent/thread/contexts.hpp"

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
            //! use shared thread contexts to setup resources
            //
            //
            //__________________________________________________________________
            class Frames
            {
            public:
                typedef Memory::Dyadic Model; //!< Memory Model


                virtual ~Frames() noexcept;

            protected:
                //! setup with pointer conversion
                /**
                 DERIVED can be Loop/Mono/Crew or Pipeline/Alone/Queue
                 */
                template <typename DERIVED> inline
                explicit Frames(const ArcPtr<DERIVED> &stc) noexcept :
                contexts(CopyOf,stc)
                {
                }
                

                //! local shared contexts
                ArcPtr<ThreadContexts>  contexts;



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Frames);
            };
        }

    }

}

#endif
