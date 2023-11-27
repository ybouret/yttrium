
#include "y/concurrent/thread/handle-zip.hpp"
#include "y/memory/buffer/ops.hpp"
#include "y/concurrent/thread.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        ThreadHandleZip:: ~ThreadHandleZip() noexcept {}

        ThreadHandleZip:: ThreadHandleZip() noexcept :
        ThreadHandleList(),
        primary( Thread::CurrentHandle() )
        {
            
        }

        size_t ThreadHandleZip:: homology() const noexcept
        {
            return Memory::BufferOps::Homology(*this);
        }

        void   ThreadHandleZip:: loadPrimary()
        {
            (*this) << primary;
        }


    }

}
