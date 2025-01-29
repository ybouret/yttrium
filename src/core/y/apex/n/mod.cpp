
#include "y/apex/natural.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        Natural operator%(const Natural &lhs, const Natural &rhs)
        {
            return Natural::Mod(lhs,rhs);
        }

        Natural operator%(const natural_t lhs, const Natural &rhs)
        {
            const Natural _(lhs);
            return Natural::Mod(_,rhs);
        }


        Natural operator%(const Natural &lhs, const natural_t rhs)
        {
            const Natural _(rhs);
            return Natural::Mod(lhs,_);
        }

        Natural & Natural:: operator%=(const Natural &rhs)
        {
            { Natural res = Mod(*this,rhs); xch(res); }
            return *this;
        }

        Natural & Natural:: operator%=(const natural_t rhs)
        {
            { const Natural _(rhs); Natural res = Mod(*this,_); xch(res); }
            return *this;
        }


        Natural Natural:: Mod(const Natural &num, const Natural &den)
        {
            DivModArg retMod = { 0 };
            return DivMod(num,den,&retMod);
        }

    }

}

