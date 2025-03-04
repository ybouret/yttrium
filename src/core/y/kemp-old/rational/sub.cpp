#include "y/kemp/rational.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        Rational Rational:: Sub(const Rational &lhs, const Rational &rhs)
        {
            const Integer num = lhs.numer * rhs.denom - rhs.numer * lhs.denom;
            const Natural den = rhs.denom * lhs.denom;
            return Rational(num,den);
        }



        Rational  Rational:: Sub(const Rational &lhs, const Integer &rhs)
        {
            const Integer num = lhs.numer - rhs * lhs.denom;
            return Rational(num,lhs.denom);
        }

        Rational Rational:: Sub(const Rational &lhs, const Natural &rhs)
        {
            const Integer num = lhs.numer - rhs * lhs.denom;
            return Rational(num,lhs.denom);
        }

        Rational Rational:: Sub(const Rational &lhs, const int64_t  rhs)
        {
            const Integer r   = rhs;
            const Integer num = lhs.numer - r * lhs.denom;
            return Rational(num,lhs.denom);
        }


        Rational Rational:: Sub(const Integer  &lhs, const Rational &rhs)
        {
            const Integer num = lhs * rhs.denom - rhs.numer;
            return Rational(num,rhs.denom);
        }

        Rational Rational:: Sub(const Natural  &lhs, const Rational &rhs)
        {
            const Integer num = lhs * rhs.denom - rhs.numer;
            return Rational(num,rhs.denom);
        }


        Rational Rational:: Sub(const int64_t   lhs, const Rational &rhs)
        {
            const Integer l   = lhs;
            const Integer num = l * rhs.denom - rhs.numer;
            return Rational(num,rhs.denom); 
        }


        void Rational:: decr()
        {
            const Integer num = numer - denom;
            Rational      tmp(num,denom);
            xch(tmp);
        }

        Rational & Rational:: operator--()
        {
            decr();
            return *this;
        }

        Rational Rational:: operator--(int)
        {
            const Rational temp(*this);
            decr();
            return temp;
        }

    }
}


