#include "y/apex/rational.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Rational Rational:: operator+() const
        {
            return *this;
        }

        void Rational:: incr()
        {
            switch(numer.s)
            {
                case __Zero__: {
                    assert(1==denom);
                    Coerce(numer) = 1;
                } break;

                case Positive:
                    Coerce(numer.n) += denom; 
                    assert( 1 == Natural::GCD(numer.n,denom) );
                    break;

                case Negative: {
                    Integer num = numer + denom;
                    switch(num.s)
                    {
                        case __Zero__: Coerce(denom) = 1;  break; // use num==0
                        case Negative:
                        case Positive:
                            break;
                    }
                    Coerce(numer).xch(num);
                    assert( 1 == Natural::GCD(numer.n,denom) );
                } break;
            }
        }

        Rational & Rational:: operator++()
        {
            incr();
            return *this;
        }

        Rational Rational:: operator++(int)
        {
            const Rational old(*this);
            incr();
            return old;
        }

    }
    
}

namespace Yttrium
{
    namespace Apex
    {
        Rational Rational:: Add(const Rational &a, const Rational &b)
        {
            const Integer num = a.numer * b.denom + b.numer * a.denom;
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

        static inline Rational FormQ(const Integer &num, const Natural &den)
        {
            switch( num.s )
            {
                case __Zero__: return Rational(0);
                case Negative:
                case Positive:
                    break;
            }
            return Rational(num,den);
        }

        Rational Rational:: Add(const Rational &a, const Integer &b)
        {
            const Integer num = a.numer + b * a.denom;
            return FormQ(num,a.denom);
        }

        Rational Rational:: Add(const Integer &a, const Rational &b)
        {
            return Add(b,a);
        }
    }

}

namespace Yttrium
{
    namespace Apex
    {


        Rational Rational:: Add(const Rational &a, const integer_t b)
        {
            switch( Sign::Of(b) )
            {
                case __Zero__: break;
                case Positive: { const Integer num = a.numer + (static_cast<natural_t>(b)  * a.denom); return FormQ(num,a.denom); }
                case Negative: { const Integer num = a.numer - (static_cast<natural_t>(-b) * a.denom); return FormQ(num,a.denom); }
            }
            return a;
        }

        Rational Rational:: Add(const integer_t a, const Rational &b)
        {
            return Add(b,a);
        }
    }

}


namespace Yttrium
{
    namespace Apex
    {
        Rational Rational:: Add(const Rational &a, const Natural &b)
        {
            switch(b->bits)
            {
                case 0: return a;
                case 1: { Rational _(a); _.incr(); return _; }
                default:
                    break;
            }
            assert(b>0);
            const Integer num = a.numer + (b*a.denom);
            return FormQ(num,a.denom);
        }


        Rational Rational:: Add(const Natural &a, const Rational &b)
        {
            return Add(b,a);
        }
    }
}
