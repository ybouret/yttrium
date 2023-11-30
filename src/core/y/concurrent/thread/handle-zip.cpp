
#include "y/concurrent/thread/handle-zip.hpp"
#include "y/memory/buffer/ops.hpp"
#include "y/concurrent/thread.hpp"

#include <cstring>

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

        ThreadHandleZip:: ThreadHandleZip(const size_t n) :
        ThreadHandleList(),
        primary( Thread::CurrentHandle() )
        {
            reserve(n);
        }


        size_t ThreadHandleZip:: homology() const noexcept
        {
            return Memory::BufferOps::Homology(*this);
        }

        void   ThreadHandleZip:: loadPrimary()
        {
            (*this) << primary;
        }



        namespace
        {
            static inline
            void RecomposeHandlesWithPrefix(const char * const pfx,
                                            const size_t       len,
                                            const ThreadHandleList &hl) noexcept
            {
                static const unsigned BufferSize = ThreadHandle::BufferSize;
                char tmp[BufferSize];

                assert(0!=pfx);
                assert(strlen(pfx)==len);

                //std::cerr << "recomposing handles with '" << pfx << "'" << std::endl;
                for(const ThreadHandleList::NodeType *node=hl.head;node;node=node->next)
                {
                    const ThreadHandle &h = **node;
                    assert(h.measure()+len<BufferSize);
                    memset(tmp,0,sizeof(tmp));
                    memcpy(tmp,pfx,len);
                    memcpy(tmp+len,h.ro_addr(),h.measure());
                    ThreadHandle org(CopyOf,tmp);
                    Coerce(h).swapWith(org);
                }

            }
        }

        bool  ThreadHandleZip:: compress(const size_t prefix)   noexcept
        {
            static const unsigned BufferSize = ThreadHandle::BufferSize;
            assert(prefix<BufferSize);

            if(size>0)
            {
                noMultiple();
                //______________________________________________________________
                //
                // storing prefix
                //______________________________________________________________
                char pfx[BufferSize] = { 0 };
                memset(pfx,0,sizeof(pfx));
                memcpy(pfx,(**head).c_str(),prefix);

                //______________________________________________________________
                //
                // removing all prefixes
                //______________________________________________________________
                for(NodeType *node=head;node;node=node->next)
                {
                    const ThreadHandle &h = **node;
                    //std::cerr << "removing " << pfx << " from " << h.c_str() << std::endl;
                    assert( 0 == memcmp(pfx,h.c_str(),prefix) );
                    Coerce(h).skip(prefix);
                }

                //______________________________________________________________
                //
                // check all new handles are different
                //______________________________________________________________
                for(const NodeType *lhs=head;lhs;lhs=lhs->next)
                {
                    for(const NodeType *rhs=lhs->next;rhs;rhs=rhs->next)
                    {
                        if( **lhs == **rhs )
                        {
                            //std::cerr << "produced same handles!!" << std::endl;
                            RecomposeHandlesWithPrefix(pfx,prefix,*this);
                            return false;
                        }
                    }
                }

            }

            return true;

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

        bool ThreadHandleZip:: compress( )   noexcept
        {
            return compress( homology() );
        }

    }

}
