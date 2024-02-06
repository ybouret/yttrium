
#include "y/memory/embedded.hpp"
#include "y/memory/allocator.hpp"

namespace Yttrium
{
    namespace Memory
    {
        

        Embedded:: Embedded(Embedding::Data &emb,
                            Allocator       &mgr) :
        alloc( &mgr ),
        bytes( 0 ),
        entry( Embed::Build( emb.data, emb.size, mgr, Coerce(bytes)) )
        {
        }


        Embedded:: Embedded(Embed * const emb,
                            const size_t  num,
                            Allocator    &mgr) :
        alloc( &mgr ),
        bytes( 0 ),
        entry( Embed::Build( emb, num, mgr, Coerce(bytes)) )
        {

        }


        Embedded:: ~Embedded() noexcept
        {
            alloc->release(Coerce(entry),Coerce(bytes));
            Coerce(alloc) = 0;
        }

    }

}

