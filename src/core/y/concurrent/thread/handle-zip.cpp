
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

        void  ThreadHandleZip:: compress(const size_t prefix) const noexcept
        {
            for(const NodeType *node=head;node;node=node->next)
            {
                const ThreadHandle &h = **node;
                Coerce(h).skip(prefix);
            }
        }

        void ThreadHandleZip:: unloadPrimary() noexcept
        {
            const ThreadHandle * const lhs = &primary;
            for(NodeType *node=head;node;node=node->next)
            {
                const ThreadHandle * const rhs = & (**node);
                if(lhs == rhs)
                {
                    cutNode(node);
                    return;
                }
            }
        }

    }

}
