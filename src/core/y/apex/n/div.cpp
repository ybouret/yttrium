#include "y/apex/natural.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"
#include <cerrno>

namespace Yttrium
{
    namespace Apex
    {

        Natural operator/(const Natural &lhs, const Natural &rhs)
        {
            return Natural::Div(lhs,rhs);
        }

        Natural operator/(const natural_t lhs, const Natural &rhs)
        {
            const Natural _(lhs);
            return Natural::Div(_,rhs);
        }


        Natural operator/(const Natural &lhs, const natural_t rhs)
        {
            const Natural _(rhs);
            return Natural::Div(lhs,_);
        }

        Natural & Natural:: operator/=(const Natural &rhs)
        {
            { Natural res = Div(*this,rhs); xch(res); }
            return *this;
        }

        Natural & Natural:: operator/=(const natural_t rhs)
        {
            { const Natural _(rhs); Natural res = Div(*this,_); xch(res); }
            return *this;
        }



        Natural Natural:: Div(const Natural &num, const Natural &den)
        {
            return DivMod(num,den,0);
        }

    }

}

