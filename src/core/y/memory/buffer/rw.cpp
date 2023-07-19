
#include "y/memory/buffer/rw.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Memory
    {
        ReadWriteBuffer:: ReadWriteBuffer() noexcept : ReadOnlyBuffer()
        {
        }

        ReadWriteBuffer:: ~ReadWriteBuffer() noexcept
        {
        }

        void * ReadWriteBuffer:: rw_addr() noexcept
        {
            return (void *) ro_addr();
        }

        

    }
}
