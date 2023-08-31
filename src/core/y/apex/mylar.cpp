#include "y/apex/mylar.hpp"

namespace Yttrium
{
    namespace Apex
    {

        const Natural & Mylar:: Dispatch(size_t &numPos, size_t &numNeg, const apq &q) noexcept
        {
            switch(q.numer.s)
            {
                case Negative: ++numNeg; break;
                case Positive: ++numPos; break;
                case __Zero__: break;
            }
            return q.denom;
        }

    }
}
