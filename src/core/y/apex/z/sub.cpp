
#include "y/apex/integer.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Integer Integer:: operator-() const
        {
            return Integer( Sign::Opposite(s), n);
        }


        Integer & Integer:: operator--()
        {
            Integer tmp = decr();
            xch(tmp);
            return *this;
        }

        Integer Integer:: operator--(int)
        {
            const Integer res = *this;
            Integer tmp = decr();
            xch(tmp);
            return res;
        }

        Integer operator-(const Integer &lhs, const Integer &rhs)
        {
            const Integer tmp = -rhs;
            return lhs+tmp;
        }


        Integer operator-(const Integer &lhs, const int64_t rhs)
        {
            const Integer tmp(rhs);
            return lhs - tmp;
        }

        Integer operator-(const int64_t lhs, const Integer &rhs)
        {
            const Integer tmp(lhs);
            return tmp - rhs;
        }

        Integer operator-(const Integer &lhs, const Natural &rhs)
        {
            const Integer tmp(rhs);
            return lhs-tmp;
        }

        Integer operator-(const Natural &lhs, const Integer &rhs)
        {
            const Integer tmp(lhs);
            return tmp-rhs;
        }


        Integer & Integer :: operator-=(const Integer &rhs)
        {
            Integer tmp = (*this) - rhs;
            xch(tmp);
            return *this;
        }

        Integer & Integer :: operator-=(const int64_t rhs)
        {
            Integer tmp = (*this) - rhs;
            xch(tmp);
            return *this;
        }

        Integer & Integer :: operator+=(const Natural &rhs)
        {
            Integer tmp = (*this) - rhs;
            xch(tmp);
            return *this;
        }

    }

}

