

#include "y/kemp/integer.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        Integer Integer:: Mul(const Integer &lhs, const Integer &rhs)
        {
            return Mul(lhs.s,lhs.n,rhs.s,rhs.n);
        }

        Integer Integer:: Mul(const Integer &lhs, const Natural &rhs)
        {
            return Mul(lhs.s,lhs.n,rhs.sign(),rhs);
        }

        Integer Integer:: Mul(const Natural &lhs, const Integer &rhs)
        {
            return Mul(lhs.sign(),lhs,rhs.s,rhs.n);
        }


        Integer Integer:: Mul(const Integer &lhs, const int64_t rhs)
        {
            switch( Sign::Of(rhs) )
            {
                case __Zero__: break;
                case Positive: { const uint64_t u = static_cast<uint64_t>( rhs); return Mul(lhs.s,lhs.n,Positive,u); }
                case Negative: { const uint64_t u = static_cast<uint64_t>(-rhs); return Mul(lhs.s,lhs.n,Negative,u); };
            }
            return lhs;
        }

        Integer Integer:: Mul(const int64_t lhs, const Integer &rhs)
        {
            switch( Sign::Of(lhs) )
            {
                case __Zero__: break;
                case Positive: { const uint64_t u = static_cast<uint64_t>( lhs);  return Mul(Positive,u,rhs.s,rhs.n); }
                case Negative: { const uint64_t u = static_cast<uint64_t>(-lhs);  return Mul(Negative,u,rhs.s,rhs.n); }
            }
            return rhs;
        }
    }

}
