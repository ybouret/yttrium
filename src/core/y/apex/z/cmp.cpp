
#include "y/apex/integer.hpp"

namespace Yttrium
{

    namespace Apex
    {
        bool operator==(const Integer &lhs, const Integer &rhs) noexcept
        {
            return (lhs.s == rhs.s) && (lhs.n == rhs.n);
        }

        static inline bool areEq(const Integer & lhs, const integer_t &rhs) noexcept
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

        bool operator==(const Integer & lhs, const integer_t rhs) noexcept
        {
            return areEq(lhs,rhs);
        }


        bool operator==(const integer_t lhs, const Integer &rhs) noexcept
        {
            return areEq(rhs,lhs);
        }

        static inline bool areEq(const Integer & lhs, const Natural & rhs) noexcept
        {
            switch(lhs.s)
            {
                case Negative: return false;
                case Positive:
                case __Zero__:
                    break;
            }
            return lhs.n == rhs;

        }

        bool operator==(const Integer & lhs, const Natural & rhs) noexcept
        {
            return areEq(lhs,rhs);
        }


        bool operator==(const Natural & lhs, const Integer & rhs) noexcept
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

        bool operator!=(const Integer & lhs, const integer_t rhs) noexcept
        {
            return !areEq(lhs,rhs);
        }


        bool operator!=(const integer_t lhs, const Integer &rhs) noexcept
        {
            return !areEq(rhs,lhs);
        }



        bool operator!=(const Integer & lhs, const Natural & rhs) noexcept
        {
            return !areEq(lhs,rhs);
        }


        bool operator!=(const Natural & lhs, const Integer & rhs) noexcept
        {
            return !areEq(rhs,lhs);
        }

    }

}

namespace Yttrium
{

    namespace Apex
    {

        SignType Integer::Compare(const Integer & lhs, const Integer & rhs) noexcept
        {
            switch( Sign::MakePair(lhs.s,rhs.s) )
            {
                case NP_Signs:
                case NZ_Signs:
                case ZP_Signs:
                    return Negative;

                case PN_Signs:
                case PZ_Signs:
                case ZN_Signs:
                    return Positive;

                case NN_Signs: return Natural::Compare(rhs.n,lhs.n); // opposite compare n
                case PP_Signs: return Natural::Compare(lhs.n,rhs.n); // directly compare n
                case ZZ_Signs: break;
            }
            return __Zero__;
        }

        SignType Integer:: Compare(const Integer & lhs, const integer_t rhs) noexcept
        {
            switch( Sign::MakePair(lhs.s, Sign::Of(rhs) ) )
            {
                case NP_Signs:
                case NZ_Signs:
                case ZP_Signs:
                    return Negative;

                case PN_Signs:
                case PZ_Signs:
                case ZN_Signs:
                    return Positive;

                case NN_Signs: return Natural::Compare(rhs,lhs.n); // opposite compare n
                case PP_Signs: return Natural::Compare(lhs.n,rhs); // directly compare n
                case ZZ_Signs: break;
            }
            return __Zero__;
        }

        SignType Integer:: Compare(const integer_t lhs, const Integer & rhs) noexcept
        {
            switch( Sign::MakePair( Sign::Of(lhs),rhs.s) )
            {
                case NP_Signs:
                case NZ_Signs:
                case ZP_Signs:
                    return Negative;

                case PN_Signs:
                case PZ_Signs:
                case ZN_Signs:
                    return Positive;

                case NN_Signs: return Natural::Compare(rhs.n,lhs); // opposite compare n
                case PP_Signs: return Natural::Compare(lhs,rhs.n); // directly compare n
                case ZZ_Signs: break;
            }
            return __Zero__;
        }

    }

}
