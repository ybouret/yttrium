

#include "y/kemp/natural.hpp"
#include "y/kemp/element.hpp"

namespace Yttrium
{
    namespace Kemp
    {


        Natural & Natural:: operator*=(const Natural &rhs)
        {
            make( Element::Mul[Strategy]( **this, *rhs) );
            return *this;
        }

        Natural & Natural:: operator*=(const uint64_t rhs)
        {
            make( Element::Mul[Strategy]( **this, rhs) );
            return *this;
        }


        Natural operator*(const Natural &lhs, const Natural &rhs)
        {
            return Natural(Element::Mul[Natural::Strategy](*lhs,*rhs),AsElement);
        }

        Natural operator*(const uint64_t lhs, const Natural &rhs)
        {
            return Natural(Element::Mul[Natural::Strategy](lhs,*rhs),AsElement);
        }

        Natural operator*(const Natural &lhs, const uint64_t rhs)
        {
            return Natural(Element::Mul[Natural::Strategy](*lhs,rhs),AsElement);
        }

    }


}
