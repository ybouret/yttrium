#include "y/kemp/integer.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        SignType Integer::Compare(const Integer &lhs, const Integer &rhs) noexcept
        {
            return Cmp(lhs.s,lhs.n,rhs.s,rhs.n);
        }

        SignType Integer:: Compare(const Integer &lhs, const Natural &rhs) noexcept
        {
            return Cmp(lhs.s,lhs.n,rhs.sign(),rhs);
        }

        SignType Integer:: Compare(const Natural &lhs, const Integer &rhs) noexcept
        {
            return Cmp(lhs.sign(),lhs,rhs.s,rhs.n);
        }

        SignType Integer:: Compare(const Integer &lhs, const int64_t rhs) noexcept
        {
            static const uint64_t zero = 0;
            switch( Sign::Of(rhs) )
            {
                case __Zero__: break;
                case Negative: { const uint64_t u = static_cast<uint64_t>(-rhs); return Cmp(lhs.s,lhs.n,Negative,u); }
                case Positive: { const uint64_t u = static_cast<uint64_t>( rhs); return Cmp(lhs.s,lhs.n,Positive,u); }
            }
            return Cmp(lhs.s,lhs.n,__Zero__,zero);
        }

        SignType Integer:: Compare(const int64_t lhs, const Integer &rhs) noexcept
        {
            static const uint64_t zero = 0;
            switch( Sign::Of(lhs) )
            {
                case __Zero__: break;
                case Negative: { const uint64_t u = static_cast<uint64_t>(-lhs); return Cmp(Negative,u,rhs.s,rhs.n); }
                case Positive: { const uint64_t u = static_cast<uint64_t>( lhs); return Cmp(Positive,u,rhs.s,rhs.n); }
            }
            return Cmp(__Zero__,zero,rhs.s,rhs.n);
        }

    }
}
