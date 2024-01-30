
#include "y/memory/embedded.hpp"
#include "y/memory/allocator.hpp"

namespace Yttrium
{
    namespace Memory
    {
        Embedded:: Embedded(Embed        emb[],
                            const size_t num,
                            Allocator   &mgr) :
        alloc( &mgr),
        bytes(0),
        entry( Embed::Build(emb,num,mgr,Coerce(bytes)) )
        {

        }

        Embedded:: Embedded() noexcept :
        alloc(0),
        bytes(0),
        entry(0)
        {
        }

        Embedded:: ~Embedded() noexcept
        {
            if(0!=alloc)
            {
                alloc->release(Coerce(entry),Coerce(bytes));
                Coerce(alloc) = 0;
            }
        }

    }

}

