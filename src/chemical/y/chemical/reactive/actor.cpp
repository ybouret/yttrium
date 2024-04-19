
#include "y/chemical/reactive/actor.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Actor:: ~Actor() noexcept {}

        Actor:: Actor(const unsigned coef,
                      const Species &spec) noexcept :
        Object(),
        sp(spec),
        nu(coef),
        xnu(nu),
        nu1(nu-1),
        next(0),
        prev(0)
        {
            assert(nu>0);
        }

        Actor:: Actor(const Actor &a) noexcept :
        Object(),
        sp(a.sp),
        nu(a.nu),
        xnu(a.xnu),
        nu1(a.nu1),
        next(0),
        prev(0)
        {

        }

        int Actor:: charge() const noexcept
        {
            return int(nu) * sp.z;
        }


        void Actor:: fmt(String &res) const
        {
            res << '[' << sp.name << ']';
        }

        String Actor:: asFirst() const
        {
            String res;
            if(nu>1)
                res += Formatted::Get("%u",nu);
            fmt(res);
            return res;
        }

        String Actor:: asExtra() const
        {
            String res = '+';
            if(nu>1)
                res += Formatted::Get("%u",nu);
            fmt(res);
            return res;
        }


    }

}
