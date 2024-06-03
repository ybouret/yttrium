
#include "y/kemp/natural.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        // num = q*den (+rem)
        Natural Natural:: Modulo(const Natural &num, const Natural &den)
        {
            const Natural q = Divide(num,den);
            return num - q*den;
        }



        Natural & Natural:: operator%=(const Natural &rhs)
        {
            Natural res = Modulo(*this,rhs);
            xch(res);
            return *this;
        }

        Natural & Natural:: operator%=(const uint64_t rhs)
        {
            const Natural den(rhs);
            return *this %= den;
        }


        Natural operator%(const Natural &lhs, const Natural &rhs)
        {
            return Natural::Modulo(lhs,rhs);
        }

        Natural operator%(const uint64_t lhs, const Natural &rhs)
        {
            const Natural num(lhs);
            return Natural::Modulo(num,rhs);
        }

        Natural operator%(const Natural &lhs, const uint64_t rhs)
        {
            const Natural den(rhs);
            return Natural::Modulo(lhs,den);
        }

    }
}
