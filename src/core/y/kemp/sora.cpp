#include "y/kemp/sora.hpp"
#include "y/container/cxx/array.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        apq & Sora:: UpdateGCD(Natural &g, const apq &q)
        {
            const Natural &rhs = q.numer.n;
            if(rhs>0)
            {
                if(g<=0)
                    g = rhs;
                else
                    g = Natural::GCD(g,rhs);
            }
            return Coerce(q);
        }

        const Natural & Sora:: Dispatch(size_t    & numPos,
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
