
#include "y/io/stream/pack64.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/calculus/align.hpp"
#include <cstring>

namespace Yttrium
{
    namespace IO
    {

        Pack64:: ~Pack64() noexcept
        {
            memset(data,0,sizeof(data));
            
        }

        uint16_t Pack64:: BytesFor(const uint64_t qw) noexcept
        {
            const  uint16_t bits = 4 + BitCount::For(qw);
            return uint16_t(Y_ALIGN_ON(8,bits)/8);
        }

        Pack64:: Pack64(const uint64_t qw) noexcept:
        size(0),
        data()
        {
            memset(data,0,sizeof(data));
            const uint16_t dataBits = uint16_t(BitCount::For(qw));
            
        }


    }
}
