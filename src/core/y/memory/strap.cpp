#include "y/memory/strap.hpp"
#include "y/check/static.hpp"

namespace Yttrium
{

    namespace Memory
    {


        Strap:: Strap(void *addr, const size_t size) noexcept :
        next(0),
        prev(0),
        head(0),
        tail(0)
        {
            Y_STATIC_CHECK(sizeof(Strap)==sizeof(Block),MismatchSizes);
            assert(size>=MinRequest);
            const size_t numBlocks = (size - sizeof(Block)) / sizeof(Block); assert(numBlocks>=3);
            head = static_cast<Block *>(addr)+1;
            tail = &head[numBlocks-1];

            head->next = tail;
            head->prev = 0;
            head->from = 0;    // means free
            head->size = (numBlocks-2) * sizeof(Block);

            tail->prev = head;
            tail->next = 0 ;
            tail->from = this; // means used
            tail->size = 0;    // and no length
        }

        Strap:: ~Strap() noexcept
        {
        }

    }

}

