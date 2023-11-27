
//! \file

#ifndef Y_Thread_Handle_Zip_Included
#define Y_Thread_Handle_Zip_Included 1

#include "y/concurrent/thread/handle.hpp"
#include "y/data/small/light/list/solo.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        typedef Small::SoloLightList<const ThreadHandle> ThreadHandleList;

        class ThreadHandleZip : public ThreadHandleList
        {
        public:
            explicit ThreadHandleZip() noexcept;
            virtual ~ThreadHandleZip() noexcept;

            size_t homology() const noexcept;
            void   loadPrimary();

            const ThreadHandle primary;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ThreadHandleZip);

        };

    }

}

#endif

