
#include "y/apex/integer.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Integer Integer:: operator-() const
        {
            return Integer( Sign::Opposite(s), n);
        }

        Integer operator-(const Integer &lhs, const Integer &rhs)
        {
#if 0
            switch( Sign::MakePair(lhs.s,rhs.s) )
            {
                case ZP_Signs:
                case ZN_Signs: return rhs;

                case PZ_Signs:
                case NZ_Signs: return lhs;

                case PP_Signs: {
                    const Natural Sum = lhs.n + rhs.n;
                    return Integer(Positive,Sum);
                }

                case NN_Signs: {
                    const Natural Sum = lhs.n + rhs.n;
                    return Integer(Negative,Sum);
                }

                case PN_Signs:
                    switch( Natural::Compare(lhs.n,rhs.n) )
                    {
                        case __Zero__: break; // will return 0
                        case Positive: { const Natural Del = lhs.n - rhs.n; return Integer(Positive,Del); }
                        case Negative: { const Natural Del = rhs.n - lhs.n; return Integer(Negative,Del); }
                    }
                    break;

                case NP_Signs:
                    switch( Natural::Compare(lhs.n,rhs.n) )
                    {
                        case __Zero__: break; // will return 0
                        case Positive: { const Natural Del = lhs.n - rhs.n; return Integer(Negative,Del); }
                        case Negative: { const Natural Del = rhs.n - lhs.n; return Integer(Positive,Del); }
                    }
                    break;

                case ZZ_Signs:
                    break;
            }
#endif
            return Integer();
        }


        Integer operator-(const Integer &lhs, const int64_t rhs)
        {
            const Integer tmp(rhs);
            return lhs - tmp;
        }

        Integer operator-(const int64_t lhs, const Integer &rhs)
        {
            const Integer tmp(lhs);
            return tmp - rhs;
        }

        Integer operator-(const Integer &lhs, const Natural &rhs)
        {
            const Integer tmp(rhs);
            return lhs-tmp;
        }

        Integer operator-(const Natural &lhs, const Integer &rhs)
        {
            const Integer tmp(lhs);
            return tmp-rhs;
        }


        Integer & Integer :: operator-=(const Integer &rhs)
        {
            Integer tmp = (*this) - rhs;
            xch(tmp);
            return *this;
        }

        Integer & Integer :: operator-=(const int64_t rhs)
        {
            Integer tmp = (*this) - rhs;
            xch(tmp);
            return *this;
        }

        Integer & Integer :: operator+=(const Natural &rhs)
        {
            Integer tmp = (*this) - rhs;
            xch(tmp);
            return *this;
        }

    }

}

