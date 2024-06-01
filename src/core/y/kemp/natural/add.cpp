
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
            Element *res = Element::Add[Strategy]( **this, *rhs);
            Swap(res,code);
            delete res;
            return *this;
        }

        Natural & Natural:: operator+=(const uint64_t rhs)
        {
            Element *res = Element::Add[Strategy]( **this, rhs);
            Swap(res,code);
            delete res;
            return *this;
        }

        void Natural:: incr()
        {
            std::cerr << "incr(" << *this << ") code=" << *code << std::endl;
            Element *res = Element::Add[Strategy]( **this, 1);
            std::cerr << "res=" << *res << std::endl;
            Swap(res,code);
            std::cerr << "old =" << *res  << std::endl;
            std::cerr << "code=" << *code << " @state=" << code->state << std::endl;
            delete res;
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
