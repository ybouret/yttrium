

#include "y/kemp/natural.hpp"
#include "y/kemp/element.hpp"

namespace Yttrium
{
    namespace Kemp
    {



        Natural & Natural:: operator-=(const Natural &rhs)
        {
            make( Element::Sub[Strategy]( **this, *rhs) );
            return *this;
        }

        Natural & Natural:: operator-=(const uint64_t rhs)
        {
            make( Element::Sub[Strategy]( **this, rhs) );
            return *this;
        }

        void Natural:: decr()
        {
            make(Element::Sub[Strategy]( **this, 1));
        }

        Natural & Natural:: operator--()
        {
            decr();
            return *this;
        }

        Natural Natural:: operator--(int)
        {
            const Natural temp(*this);
            decr();
            return temp;
        }

        Natural operator-(const Natural &lhs, const Natural &rhs)
        {
            return Natural(Element::Sub[Natural::Strategy](*lhs,*rhs),AsElement);
        }

        Natural operator-(const uint64_t lhs, const Natural &rhs)
        {
            return Natural(Element::Sub[Natural::Strategy](lhs,*rhs),AsElement);
        }

        Natural operator-(const Natural &lhs, const uint64_t rhs)
        {
            return Natural(Element::Sub[Natural::Strategy](*lhs,rhs),AsElement);
        }

    }


}
