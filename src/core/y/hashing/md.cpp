
#include "y/hashing/md.hpp"

#include "y/hashing/function.hpp"
#include "y/memory/digest.hpp"

namespace Yttrium
{

    namespace Hashing
    {
        Digest MD::Of(Function &H)
        {
            const size_t n = H.length;
            Digest       d(n);
            H.get(&d[1],n);
            return d;
        }

        Digest MD::Of(Function &H, const void *block_addr, const size_t block_size)
        {
            const size_t n = H.length;
            Digest       d(n);
            H.block(&d[1],n,block_addr,block_size);
            return d;
        }

        Digest MD::Of(Function &H, const char *msg)
        {
            const size_t n = H.length;
            Digest       d(n);
            H.block(&d[1],n,msg);
            return d;
        }

        Digest MD::Of(Function &H, const Memory::ReadOnlyBuffer &buf)
        {
            const size_t n = H.length;
            Digest       d(n);
            H.block(&d[1],n,buf);
            return d;
        }

    }

}
