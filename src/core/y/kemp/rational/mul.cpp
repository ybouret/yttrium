#include "y/kemp/rational.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        static inline
        Rational MulAs(const SignType s, const Rational &lhs, const Rational &rhs)
        {
            const Natural n = lhs.numer.n * rhs.numer.n;
            const Natural d = lhs.denom * rhs.denom;
            return Rational(s,n,d);
        }

        Rational Rational:: Mul(const Rational &lhs, const Rational &rhs)
        {

            switch( Sign::MakePair(lhs.numer.s,rhs.numer.s) )
            {
                    // trivial cases
                case ZZ_Signs:
                case ZN_Signs:
                case ZP_Signs:
                case NZ_Signs:
                case PZ_Signs:
                    break;

                    // positive
                case NN_Signs:
                case PP_Signs: return MulAs(Positive,lhs,rhs);

                    // negative
                case NP_Signs:
                case PN_Signs: return MulAs(Negative,lhs,rhs);


            }
            return Rational();
        }

        Rational Rational:: Mul(const Rational &lhs, const Integer &rhs)
        {
            const Integer num = lhs.numer * rhs;
            return Rational(num,lhs.denom);
        }

        Rational Rational:: Mul(const Rational &lhs, const Natural &rhs)
        {
            const Integer num = lhs.numer * rhs;
            return Rational(num,lhs.denom);
        }

        Rational Rational:: Mul(const Rational &lhs, const int64_t rhs)
        {
            const Integer num = lhs.numer * rhs;
            return Rational(num,lhs.denom);
        }

        Rational Rational:: Mul(const Integer  &lhs, const Rational &rhs)
        {
            return Mul(rhs,lhs);
        }

        Rational Rational:: Mul(const Natural  &lhs, const Rational &rhs)
        {
            return Mul(rhs,lhs);
        }

        Rational Rational:: Mul(const int64_t   lhs, const Rational &rhs)
        {
            return Mul(rhs,lhs);
        }

    }

}

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
    }

}
