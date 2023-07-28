
#include "y/io/libc/iobuffer.hpp"
#include "y/calculus/base2.hpp"
#include "y/memory/allocator/dyadic.hpp"

#include <cstdio>

namespace Yttrium
{

    namespace Libc
    {

        static inline char * MemAcquire(size_t &bytes, unsigned &shift)
        {
            static Memory::Dyadic &mgr = Memory::Dyadic::Instance();

            void *res = mgr.acquireBlock(shift);
            bytes = size_t(1) << shift;
            return static_cast<char *>(res);
        }

        IOBuffer:: IOBuffer() :
        bytes( BUFSIZ ),
        shift( Base2<size_t>::LogFor( Coerce(bytes) )  ),
        chars( MemAcquire(Coerce(bytes),Coerce(shift)) )
        {
        }

        IOBuffer:: ~IOBuffer() noexcept
        {
            
        }

    }

}
