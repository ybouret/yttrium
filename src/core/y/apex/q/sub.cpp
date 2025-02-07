
#include "y/apex/rational.hpp"
#include "y/system/exception.hpp"

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
