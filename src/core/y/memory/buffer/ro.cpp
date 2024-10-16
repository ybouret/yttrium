#include "y/memory/buffer/ro.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/stream/output.hpp"
#include "y/type/utils.hpp"
#include "y/check/crc32.hpp"

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

        uint32_t ReadOnlyBuffer:: crc32() const noexcept
        {
            return CRC32:: Of( ro_addr(), measure() );
        }



        bool ReadOnlyBuffer:: hasSameContentThan(const ReadOnlyBuffer &other) const noexcept
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


        size_t ReadOnlyBuffer:: maxSameContentThan(const ReadOnlyBuffer &other) const noexcept
        {
            const uint8_t *lhs = static_cast<const uint8_t *>( ro_addr() );
            const uint8_t *rhs = static_cast<const uint8_t *>( other.ro_addr() );
            size_t         ans = 0;
            for(size_t i=Min( measure(), other.measure());i>0;--i,++lhs,++rhs,++ans )
            {
                if( *lhs != *rhs ) break;
            }
            return ans;
        }

    }
}
