#include "y/kemp/rational.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        Rational Rational::Square(const Rational &q)
        {
            const Natural n2 = q.numer.n.square();
            const Natural d2 = q.denom.square();
            return Rational(n2,d2);
        }

        Rational Rational:: square() const
        {
            return Square(*this);
        }

        Rational Rational::Abs(const Rational &q)
        {
            Rational a = q;
            Sign::ReplaceByAbs( Coerce(a.numer.s) );
            return a;
        }

        Rational Rational:: abs() const
        {
            return Abs(*this);
        }

        Rational Rational:: Sqrt(const Rational &q)
        {
            const Integer u   = q.numer.sqrt();
            const Natural du2 = q.denom * u.n * u.n;
            Natural       v   = q.denom.sqrt();
            while(v*v*q.numer.n<du2) ++v;
            return Rational(u,v);
        }

        Rational Rational:: sqrt() const
        {
            return Sqrt(*this);
        }
    }

}
