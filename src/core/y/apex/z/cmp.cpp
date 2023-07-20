
#include "y/apex/integer.hpp"

namespace Yttrium
{
    namespace Apex
    {

        SignType Integer:: Compare(const Integer &lhs, const Integer &rhs) noexcept
        {

            switch( Sign::MakePair(lhs.s,rhs.s) )
            {
                case NZ_Signs:
                case ZP_Signs:
                case NP_Signs:
                    return Negative;
                case PN_Signs:
                case ZN_Signs:
                case PZ_Signs:
                    return Positive;
                case PP_Signs: return Natural::Compare(lhs.n,rhs.n);
                case NN_Signs: return Natural::Compare(rhs.n,lhs.n);
                case ZZ_Signs: break;
            }
            return __Zero__;
        }

        bool operator==(const Integer &lhs, const Integer &rhs) noexcept
        {
            return (lhs.s==rhs.s) && (lhs.n == rhs.n);
        }

        bool operator!=(const Integer &lhs, const Integer &rhs) noexcept
        {
            return (lhs.s!=rhs.s) && (lhs.n != rhs.n);
        }

    }

}


