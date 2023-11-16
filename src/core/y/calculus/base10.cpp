
#include "y/calculus/base10.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    unsigned Base10:: DigitsForU64(uint64_t u) noexcept
    {

        unsigned res = 1;
        while(true)
        {
            const uint64_t quot = u / 10;
            if(quot<=0) break;
            ++res;
            u = quot;
        }

        return Max<unsigned>(res,1);
    }

}

