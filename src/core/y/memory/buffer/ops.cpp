#include "y/memory/buffer/ops.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{

    namespace Memory
    {
#if 0
        static inline size_t homology(const BufferOps::BufferNode *lhs,
                                      const BufferOps::BufferNode *rhs) noexcept
        {
            assert(0!=lhs);
            assert(0!=rhs);
            return (**lhs).maxSameContentThan(**rhs);
        }


        size_t BufferOps:: Homology(const BufferList &blist) noexcept
        {
            size_t len = 0;
            if(blist.size>=2)
            {
                // initialize
                assert(0!=blist.head);
                assert(0!=blist.head->next);
                len = homology(blist.head,blist.head->next);

                // loop do decrease homology
                for(const BufferNode *lhs=blist.head;lhs;lhs=lhs->next)
                {
                    for(const BufferNode *rhs=lhs->next;rhs;rhs=rhs->next)
                    {
                        len = Min(len,homology(lhs,rhs));
                    }
                }

            }

            return len;
        }
#endif
        
        size_t BufferOps:: PairHomology(const ReadOnlyBuffer &lhs,
                                        const ReadOnlyBuffer &rhs) noexcept
        {
            return lhs.maxSameContentThan(rhs);
        }

    }
}
