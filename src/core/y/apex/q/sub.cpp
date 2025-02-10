#include "y/apex/rational.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Rational Rational:: operator-() const
        {
            Rational _(*this);
            Sign::ReplaceByOpposite( Coerce(_.numer.s) );
            return _;
        }

        void Rational:: decr()
        {
            switch(numer.s)
            {
                case Negative:
                    Coerce(numer.n) += denom;
                    assert(1==Natural::GCD(numer.n,denom));
                    break;

                case __Zero__:
                    assert(1==denom);
                    Coerce(numer) = -1;
                    break;

                case Positive: {
                    Coerce(numer) -= denom;
                    assert(1==Natural::GCD(numer.n,denom));
                } break;
            }
        }

        Rational & Rational:: operator--()
        {
            decr();
            return *this;
        }

        Rational Rational:: operator--(int)
        {
            const Rational old(*this);
            decr();
            return old;
        }
    }

}

namespace Yttrium
{
    namespace Apex
    {


        Rational Rational:: Sub(const Rational &a, const Rational &b)
        {
            const Integer num = a.numer * b.denom - b.numer * a.denom;
            switch( num.s )
            {
                case __Zero__: return Rational(0);
                case Negative:
                case Positive:
                    break;
            }
            const Natural den = a.denom * b.denom;
            return Rational(num,den);
        }
    }

}


namespace Yttrium
{
    namespace Apex
    {
        Rational Rational:: Sub(const Rational &a, const Integer &b)
        {
            const Integer num = a.numer - b * a.denom;
            switch( num.s )
            {
                case __Zero__: return Rational(0);
                case Negative:
                case Positive:
                    break;
            }
            return Rational(num,a.denom);
        }

        Rational Rational:: Sub(const Integer &a, const Rational &b)
        {
            Rational _( Sub(b,a) );
            Sign::ReplaceByOpposite( Coerce(_.numer.s) );
            return _;
        }
    }

}


namespace Yttrium
{
    namespace Apex
    {
        Rational Rational:: Sub(const Rational &a, const integer_t b)
        {
            return Add(a,-b);
        }

        Rational Rational:: Sub(const integer_t a, const Rational &b)
        {
            Rational _ = Add(-a,b);
            Sign::ReplaceByOpposite( Coerce(_.numer.s) );
            return _;
        }

    }

}

namespace Yttrium
{
    namespace Apex
    {


        Rational Rational:: Sub(const Rational &a, const Natural &b)
        {
            switch(b->bits)
            {
                case 0: return a;
                case 1: { Rational _(a); _.decr(); return _; }
                default:
                    break;
            }
            assert(b>0);
            const Integer num = a.numer - (b*a.denom);
            switch(num.s)
            {
                case __Zero__: return 0;
                case Positive:
                case Negative:
                    break;
            }
            return Rational(num,a.denom);
        }


        Rational Rational:: Sub(const Natural &a, const Rational &b)
        {
            Rational _ = Sub(b,a);
            Sign::ReplaceByOpposite( Coerce(_.numer.s) );
            return _;
        }
    }
}
