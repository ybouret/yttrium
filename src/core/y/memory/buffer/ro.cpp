#include "y/memory/buffer/ro.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/io/stream/output.hpp"
#include <iostream>
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

        std::ostream & ReadOnlyBuffer:: displayHexadecimal(std::ostream &os) const
        {
            const uint8_t *p = static_cast<const uint8_t *>( ro_addr() );
            const size_t   n = measure();
            for(size_t i=0;i<n;++i)
            {
                os << Hexadecimal::Text[p[i]];
            }
            return os;
        }

        OutputStream & ReadOnlyBuffer:: displayHexadecimal(OutputStream &os) const
        {
            const uint8_t *p = static_cast<const uint8_t *>( ro_addr() );
            const size_t   n = measure();
            for(size_t i=0;i<n;++i)
            {
                os << Hexadecimal::Text[p[i]];
            }
            return os;
        }

    }
}
