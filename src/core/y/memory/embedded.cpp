
#include "y/memory/embedded.hpp"
#include "y/memory/allocator.hpp"

namespace Yttrium
{
    namespace Memory
    {
        Embedded:: Embedded(Embed        emb[],
                            const size_t num,
                            Allocator   &mgr) :
        alloc(mgr),
        bytes(0),
        entry( Embed::Build(emb,num,alloc,bytes) )
        {

        }

        Embedded:: ~Embedded() noexcept
        {
            alloc.release(entry,bytes);
        }

    }

}

