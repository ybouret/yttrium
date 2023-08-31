#include "y/apex/mylar.hpp"

namespace Yttrium
{
    namespace Apex
    {

        const Natural & Mylar:: Dispatch(size_t    & numPos,
                                         size_t    & numNeg,
                                         SignType  & firstSign,
                                         const apq & q) noexcept
        {
            const SignType s = q.numer.s;

            if(__Zero__==firstSign) firstSign = s;

            switch(q.numer.s)
            {
                case Negative:
                    ++numNeg;
                    break;
                case Positive:
                    ++numPos;
                    break;
                case __Zero__: break;
            }
            return q.denom;
        }

    }
}
