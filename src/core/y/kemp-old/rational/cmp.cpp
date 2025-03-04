
#include "y/kemp/rational.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        SignType Rational::Compare(const Rational &lhs, const Rational &rhs)
        {
            const Integer &a = lhs.numer; const Integer &c = rhs.numer;
            const Natural &b = lhs.denom; const Natural &d = rhs.denom;
            const Integer t = a*d - b*c;
            return t.s;
        }

        SignType Rational::Compare(const Rational &lhs, const Integer &c)
        {
            const Integer &a = lhs.numer;
            const Natural &b = lhs.denom;
            const Integer  t = a - c*b;
            return t.s;
        }

        SignType Rational::Compare(const Rational &lhs, const Natural &c)
        {
            const Integer &a = lhs.numer;
            const Natural &b = lhs.denom;
            const Integer  t = a - c*b;
            return t.s;
        }

        SignType Rational:: Compare(const Rational &lhs, const int64_t rhs)
        {
            const Integer  c = rhs;
            const Integer &a = lhs.numer;
            const Natural &b = lhs.denom;
            const Integer  t = a - c*b;
            return t.s;
        }

        SignType Rational::Compare(const Integer &a, const Rational &rhs)
        {
            const Integer &c = rhs.numer;
            const Natural &d = rhs.denom;
            const Integer t = a*d - c;
            return t.s;
        }

        SignType Rational::Compare(const Natural &a, const Rational &rhs)
        {
            const Integer &c = rhs.numer;
            const Natural &d = rhs.denom;
            const Integer t = a*d - c;
            return t.s;
        }

        SignType Rational::Compare(const int64_t lhs, const Rational &rhs)
        {
            const Integer  a = lhs;
            const Integer &c = rhs.numer;
            const Natural &d = rhs.denom;
            const Integer t  = a*d - c;
            return t.s;
        }
    }

}

