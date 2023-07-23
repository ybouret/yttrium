
#include "y/apex/rational.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Rational Rational:: operator-() const
        {
            Rational res = *this;
            Sign::Change( Coerce(res.numer.s) );
            return res;
        }

        Rational   operator - (const Rational &a, const Rational &b)
        {
            const Integer N = a.numer * b.denom - b.numer * a.denom;
            const Natural D = a.denom * b.denom;
            return Rational(N,D);
        }

        Rational   operator - (const Rational &a, const int64_t b)
        {
            const Integer B = -b;
            const Integer N = a.numer + B * a.denom;
            return Rational(N,a.denom);
        }

        Rational   operator - (const int64_t a, const Rational &b)
        {
            const Integer A = a;
            const Integer N = A * b.denom - b.numer;
            return Rational(N,b.denom);
        }


        Rational   operator - (const Rational &a, const Integer &b)
        {
            const Integer N = a.numer - b * a.denom;
            return Rational(N,a.denom);
        }

        Rational   operator - (const Integer &a, const Rational &b)
        {
            const Integer N = a * b.denom - b.numer;
            return Rational(N,b.denom);
        }


        Rational   operator - (const Rational &a, const Natural &b)
        {
            const Integer N = a.numer - b * a.denom;
            return Rational(N,a.denom);
        }

        Rational   operator - (const Natural &a, const Rational &b)
        {
            const Integer N = a * b.denom - b.numer;
            return Rational(N,b.denom);
        }


        Y_APQ_BINARY_IMPL(-)

    }
}
