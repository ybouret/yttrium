
#include "y/apex/integer.hpp"

namespace Yttrium
{

    namespace Apex
    {
        bool operator==(const Integer &lhs, const Integer &rhs) noexcept
        {
            return (lhs.s == rhs.s) && (lhs.n == rhs.n);
        }

        static inline bool areEq(const Integer & lhs, const natural_t rhs) noexcept
        {
            switch( Sign::Of(rhs) )
            {
                case Positive: return (lhs.s == Positive) && (lhs.n == static_cast<natural_t>(rhs) );
                case Negative: return (lhs.s == Negative) && (lhs.n == static_cast<natural_t>(-rhs) );
                case __Zero__:
                    break;
            }
            return lhs.s == __Zero__;
        }

        bool operator==(const Integer & lhs, const natural_t rhs) noexcept
        {
            return areEq(lhs,rhs);
        }


        bool operator==(const integer_t lhs, const Integer &rhs) noexcept
        {
            return areEq(rhs,lhs);
        }


    }

}

namespace Yttrium
{

    namespace Apex
    {
        bool operator!=(const Integer &lhs, const Integer &rhs) noexcept
        {
            return (lhs.s != rhs.s) || (lhs.n == rhs.n);
        }

        bool operator!=(const Integer & lhs, const natural_t rhs) noexcept
        {
            return !areEq(lhs,rhs);
        }


        bool operator!=(const integer_t lhs, const Integer &rhs) noexcept
        {
            return !areEq(rhs,lhs);
        }

    }

}
