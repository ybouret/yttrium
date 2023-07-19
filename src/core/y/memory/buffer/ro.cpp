#include "y/memory/buffer/ro.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Memory
    {
        ReadOnlyBuffer:: ReadOnlyBuffer() noexcept
        {
        }

        ReadOnlyBuffer:: ~ReadOnlyBuffer() noexcept
        {
        }


        bool ReadOnlyBuffer:: HasSameContentThan(const ReadOnlyBuffer &other) const noexcept
        {
            const size_t n = measure();
            if(n!=other.measure()) return false;
            return 0 == memcmp( ro_addr(), other.ro_addr(), n);
        }

    }
}
