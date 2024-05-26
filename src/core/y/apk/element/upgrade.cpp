#include "y/apk/element.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/calculus/align.hpp"

#include <cstring>
#include <cerrno>

namespace Yttrium
{
    namespace APK
    {
        void Element:: upgrade() noexcept
        {
            switch(state)
            {
                case AsBytes: upgradeAsBytes(); break;
                case AsNum16: upgradeAsNum16(); break;
                case AsNum32: upgradeAsNum32(); break;
                case AsNum64: upgradeAsNum64(); break;
            }
        }

        void Element:: updateFromBits() noexcept
        {
            Coerce(bytes) = ( Y_ALIGN8(bits)  / 8  );
            Coerce(num16) = ( Y_ALIGN16(bits) / 16 );
            Coerce(num32) = ( Y_ALIGN32(bits) / 32 );
            Coerce(num64) = ( Y_ALIGN64(bits) / 64 );
        }


        void Element:: upgradeAsBytes() noexcept
        {
            uint8_t * const p = static_cast<uint8_t *>(entry);
            size_t &        n = Coerce(bytes) = maxBytes;
            while(n>0)
            {
                if(p[--n]>0) break;
            }
            if(n<=0)
            {
                assert(0==bytes);
                ldz();
            }
            else
            {
                const size_t msb = n-1;
                Coerce(bits)  = (msb * 8) + BitCount::For( p[msb] );
                updateFromBits();
                memset(p+bytes,0,maxBytes-bytes);
            }
        }
    }

}


