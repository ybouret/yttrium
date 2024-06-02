
#include "y/kemp/natural.hpp"
#include "y/kemp/element.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        Natural Natural:: operator+() const
        {
            return Natural(*this);
        }

        Natural & Natural:: operator+=(const Natural &rhs)
        {
            make( Element::Add[Strategy]( **this, *rhs) );
            return *this;
        }

        Natural & Natural:: operator+=(const uint64_t rhs)
        {
            make( Element::Add[Strategy]( **this, rhs) );
            return *this;
        }

        void Natural:: incr()
        {
            make(Element::Add[Strategy]( **this, 1));
        }

        Natural & Natural:: operator++()
        {
            incr();
            return *this;
        }

        Natural Natural:: operator++(int)
        {
            const Natural temp(*this);
            incr();
            return temp;
        }

        Natural operator+(const Natural &lhs, const Natural &rhs)
        {
            return Natural(Element::Add[Natural::Strategy](*lhs,*rhs),AsElement);
        }

        Natural operator+(const uint64_t lhs, const Natural &rhs)
        {
            return Natural(Element::Add[Natural::Strategy](lhs,*rhs),AsElement);
        }

        Natural operator+(const Natural &lhs, const uint64_t rhs)
        {
            return Natural(Element::Add[Natural::Strategy](*lhs,rhs),AsElement);
        }

    }


}
