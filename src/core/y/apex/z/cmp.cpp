
#include "y/apex/integer.hpp"

namespace Yttrium
{
    namespace Apex
    {
        bool operator==(const Integer &lhs, const Integer &rhs) noexcept
        {
            return (lhs.s==rhs.s) && (lhs.n == rhs.n);
        }

        bool operator!=(const Integer &lhs, const Integer &rhs) noexcept
        {
            return (lhs.s!=rhs.s) && (lhs.n != rhs.n);
        }


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


        SignType Integer:: Cmp(const Integer &lhs, const Integer &rhs) noexcept
        {
            return Compare(lhs,rhs);
        }

        SignType Integer:: Cmp(const Integer &lhs, const int64_t rhs) noexcept
        {
            const SignType rs = Sign::Of(rhs);
            switch( Sign::MakePair(lhs.s,rs) )
            {
                case NZ_Signs:
                case ZP_Signs:
                case NP_Signs:
                    return Negative;
                    
                case PN_Signs:
                case ZN_Signs:
                case PZ_Signs:
                    return Positive;

                case PP_Signs: return Natural::Compare(lhs.n,uint64_t(rhs));
                case NN_Signs: return Natural::Compare(uint64_t(-rhs),lhs.n);
                case ZZ_Signs: break;
            }
            return __Zero__;
        }



    }

}


