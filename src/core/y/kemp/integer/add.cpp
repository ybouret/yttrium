
#include "y/kemp/integer.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        Integer Integer:: Add(const Integer &lhs, const Integer &rhs)
        {
            return Add(lhs.s,lhs.n,rhs.s,rhs.n);
        }

        Integer Integer:: Add(const Integer &lhs, const Natural &rhs)
        {
            return Add(lhs.s,lhs.n,rhs.sign(),rhs);
        }

        Integer Integer:: Add(const Natural &lhs, const Integer &rhs)
        {
            return Add(lhs.sign(),lhs,rhs.s,rhs.n);
        }


        Integer Integer:: Add(const Integer &lhs, const int64_t rhs)
        {
            switch( Sign::Of(rhs) )
            {
                case __Zero__: break;
                case Positive: { const uint64_t u = static_cast<uint64_t>( rhs); return Add(lhs.s,lhs.n,Positive,u); }
                case Negative: { const uint64_t u = static_cast<uint64_t>(-rhs); return Add(lhs.s,lhs.n,Negative,u); };
            }
            return lhs;
        }

        Integer Integer:: Add(const int64_t lhs, const Integer &rhs)
        {
            switch( Sign::Of(lhs) )
            {
                case __Zero__: break;
                case Positive: { const uint64_t u = static_cast<uint64_t>( lhs);  return Add(Positive,u,rhs.s,rhs.n); }
                case Negative: { const uint64_t u = static_cast<uint64_t>(-lhs);  return Add(Negative,u,rhs.s,rhs.n); }
            }
            return rhs;
        }
    }

}