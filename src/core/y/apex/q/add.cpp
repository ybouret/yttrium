
#include "y/apex/rational.hpp"
#include "y/system/exception.hpp"

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
                    assert(1==denom); Integer _1(1); Coerce(numer).xch(_1);
                    assert( 1 == Natural::GCD(numer.n,denom) );
                } break;

                case Positive:
                    Coerce(numer.n) += denom; 
                    assert( 1 == Natural::GCD(numer.n,denom) );
                    break;

                case Negative: {
                    Integer num = numer + denom;
                    switch(num.s)
                    {
                        case __Zero__: Coerce(denom) = 1; ; break;
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
