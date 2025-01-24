
#include "y/apex/natural.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Natural & Natural:: _shr(const size_t nbit) noexcept
        {
            if(nbit<=0) return *this;
            
            const size_t bits = block->bits;
            if(nbit>=bits) {
                block->ldz();
                return *this;
            }

            const size_t newBits = bits-nbit;
            size_t       target  = newBits;
            size_t       source  = bits;

            block->sync();
            return *this;
        }

    }

}
